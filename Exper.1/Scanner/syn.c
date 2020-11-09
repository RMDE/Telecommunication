/*****************************************************************************
** Author : Ye Xi
** ID     : 161720110
** Class  : 1617204
** Github : https://github.com/RMDE/Telecommunication/Expr.1/Scanner/syn.c
** refer to :https://github.com/venkat-abhi/Half-open-port-scanner.git
*****************************************************************************/
#include<stdio.h>
#include "syn.h"
#include<unistd.h>
#include<sys/wait.h>
#include<signal.h>

uint16_t csum(const void*,const int);
uint16_t tcp_chksum(struct my_iph*,struct my_tcph*);
void perror_exit(const char*);
extern void scan_tcp_ports(char*);
void* scanner(__attribute__((unused))void *);
void close_connection(uint16_t,struct sockaddr_storage);
void* listener(__attribute__((unused))void*);
void set_interface_ip(const char*,struct my_iph*);
void set_dest_ip(struct my_iph*);
void set_ip_hdr(struct my_iph*);
void set_tcp_hdr(struct my_tcph*);
void set_raw_socket();
void set_socket_options();
void create_thread(enum threadType,pthread_t*);


//calculate the checksum
uint16_t csum(const void *data, const int length)
{
	/*  Checksum Algorithm (http://www.microhowto.info/howto/calculate_an_internet_protocol_checksum_in_c.html)
	1. Set the sum to 0,
	2. Pad the data to an even number of bytes,
	3. Reinterpret the data as a sequence of 16-bit unsigned integers that are
		in network byte order,
	4. Calculate the sum of the integers, subtracting 0xffff whenever
		the sum => 0x10000, and
	5. Calculate the bitwise complement of the sum and set it as the checksum.
	*/
	uint16_t *accumalator = (uint16_t *)data;
	uint64_t sum = 0;

	/* Take care of the first 16-bit even blocks */
	for (int i = 0; i < length/2; ++i) {
		sum += *(accumalator+i);
		if (sum >= 0x10000) {
			sum -= 0xffff;
		}
	}

	/* Handle the ending partial block */
	if (length % 2 != 0) {
		accumalator = accumalator+ length/2; /* Point accumalator to the end block */
		uint16_t end_block = 0;
		memcpy(&end_block, accumalator, sizeof(length));
		sum += ntohs(end_block);
		if (sum >= 0x10000) {
			sum -= 0xffff;
		}
	}

	/* Return the one's complement of the checksum in network byte order */
	return htons(~sum);
}

uint16_t tcp_chksum(struct my_iph *snd_iph, struct my_tcph *snd_tcph)
{
	struct psuedo_header psh;

	psh.src_addr = snd_iph->src_addr;
	psh.dst_addr = snd_iph->dst_addr;
	psh.rsvd = 0;
	psh.proto = IPPROTO_TCP;
	psh.len_tcp = htons(sizeof(struct my_tcph));	/* No options, and no data */

	int pseudogram_size = sizeof(struct my_tcph) + sizeof(struct psuedo_header);
	char *pseudogram = malloc(pseudogram_size);

	memcpy(pseudogram, (char *)&psh, sizeof(struct psuedo_header));
	memcpy(pseudogram + sizeof(struct psuedo_header), snd_tcph, sizeof(struct my_tcph));

	return(htons(csum(pseudogram, pseudogram_size)));
	//return (htons(csum(snd_tcph, sizeof(struct my_tcph)) + csum(&psh, sizeof(struct psuedo_header))));
}

void perror_exit(const char *s)
{
	fprintf(stderr, "%s: %s\n", s, strerror(errno));
	exit(EXIT_FAILURE);
}

void scan_tcp_ports(char* ip)
{
    int pid = fork();
    if(pid==-1)
    {
        printf("process create error!\n");
        exit(0);
    }
    if(pid == 0)
    {
	    pthread_t* g_listener_thread,*g_scanner_thread;
        g_listener_thread=(pthread_t*)malloc(sizeof(pthread_t));
        g_scanner_thread=(pthread_t*)malloc(sizeof(pthread_t));
        dest_host_name = ip;
	    set_raw_socket();
	    set_socket_options();
    
	    create_thread(LISTENER_THREAD,g_listener_thread);
	    create_thread(SCANNER_THREAD,g_scanner_thread);
    	pthread_join(*g_listener_thread, NULL);
	    pthread_join(*g_scanner_thread, NULL);
        free(g_listener_thread);
        free(g_scanner_thread);
        _exit(0);
    }
    else
    {
        wait(NULL);
    }
}

void* scanner(__attribute__((unused)) void *unused)
{
	/* Packet to be sent to scan for ports */
	char scanning_packet[MAX_PCKT_LEN];
	
	/* Zero out the packet */
	memset(scanning_packet, 0 , MAX_PCKT_LEN);

	struct my_iph *snd_iph = (struct my_iph *)scanning_packet;
	struct my_tcph *snd_tcph = (struct my_tcph *)(scanning_packet + sizeof(struct my_iph));

	set_ip_hdr(snd_iph);
	set_tcp_hdr(snd_tcph);

	/* Set up the destination address struct */
	struct sockaddr_in p_dest_addr;
	memset((char *)&p_dest_addr, 0, sizeof(struct sockaddr_in));
	p_dest_addr.sin_family = AF_INET;	/* IPv4 address */
	p_dest_addr.sin_port = htons(atoi(COMMS_PORT)); 
	p_dest_addr.sin_addr.s_addr = snd_iph->dst_addr;

    //printf("Now Scan %s\n",inet_ntoa(p_dest_addr.sin_addr));

	for (int i = 1; i < 1024; ++i) {
		snd_tcph->dst_port = htons(i);
		snd_iph->hdr_chk_sum = csum(scanning_packet, snd_iph->tot_len);
		snd_tcph->chksum = tcp_chksum(snd_iph, snd_tcph);
		
		if (sendto(g_sockfd, scanning_packet, snd_iph->tot_len,
			0, (struct sockaddr *)&p_dest_addr, sizeof(p_dest_addr)) <= 0) {
			perror("sendto() error:");
			printf("fail\n");
		}

		snd_tcph->chksum = 0;

		/* Sleep before sending again */
		usleep(100);
	}

	return NULL;
}

void close_connection(uint16_t port, struct sockaddr_storage from_addr)
{
	/* Packet to be sent to close a connection to a port  */
	char closing_packet[MAX_PCKT_LEN];
	
	/* Zero out the packet */
	memset(closing_packet, 0 , MAX_PCKT_LEN);

	/* Struct to send for closing the connection */
	struct my_iph *close_iph = (struct my_iph *)closing_packet;
	struct my_tcph *close_tcph = (struct my_tcph *)(closing_packet + sizeof(struct my_iph));

	set_ip_hdr(close_iph);
	set_tcp_hdr(close_tcph);

	/* Hacky code; need to find a better approach */
	close_iph->dst_addr = ((struct sockaddr_in *)&from_addr)->sin_addr.s_addr;

	/* Close the connection to the port */
	close_tcph->fin = 0x1;	
	close_tcph->ack = 0x0;

	close_tcph->dst_port = htons(port);
	close_iph->hdr_chk_sum = csum(closing_packet, close_iph->tot_len);
	close_tcph->chksum = tcp_chksum(close_iph, close_tcph);
	
	if (sendto(g_sockfd, closing_packet, close_iph->tot_len,
		0, (struct sockaddr *)&from_addr, sizeof(from_addr)) <= 0) {
		perror("sendto()");
	}

	close_tcph->chksum = 0;
}


void* listener(__attribute__((unused)) void *unused)
{
    int count=0;
    int pid=fork();
    if(pid==0)
    {
	    for (;;) {
		    /* Packet received as reply from target */
		    char response_packet[IP_PCKT_MAX_LEN];

		    /* Zero out the buffer */
		    memset(response_packet, 0, IP_PCKT_MAX_LEN);

		    /* Holds the destination network information */
		    struct sockaddr_storage from_addr;
		    socklen_t from_len = 0;

		    /* Recieve the response from the target */
		    int byte_count = recvfrom(g_sockfd, response_packet, MAX_PCKT_LEN, 0, (struct sockaddr *)&from_addr, &from_len);
		    if (byte_count < 0 && errno != EAGAIN) {
			    perror("recvfrom: ");
			    continue;
		    }
            count++;

    		/* Get the pointers to the IP & TCP headers */
	    	struct my_iph *recv_iph = (struct my_iph*)response_packet;
		    struct my_tcph *recv_tcph = (struct my_tcph*)(response_packet + 4 * (recv_iph->hdr_len));


	    	/* Check if the message is for COMMS_PORT port */
		    if (recv_tcph->dst_port != ntohs(atoi(COMMS_PORT))) {
			    continue;
		    }

		    /* Check if we the port is closed (denoted by a rst flag) */
		    if (recv_tcph->rst == 0x01) {
			    continue;
		    }

		    /* Check if the target is closing the connection (done if we haven't responded to it's acks) */
		    if (recv_tcph->fin == 0x01) {
			    continue;
		    }

		    /* Check to see if we recived an ACK for a port */
		    if (recv_tcph->ack == 0x01 && recv_tcph->syn == 0x01) {
			    //printf("\nChecking port %d\n", ntohs(recv_tcph->src_port));
			    /* Check if ack is retransmitted due to delayed fin */
			    if (discovered_ports[ntohs(recv_tcph->src_port) == 1]) {
				    continue;
			    }
			    printf("[*] Port: %d is open.\n", ntohs(recv_tcph->src_port));

			    /* Close the connection */
			    discovered_ports[ntohs(recv_tcph->src_port)] = 1;
			    close_connection(ntohs(recv_tcph->src_port), from_addr);
		    }
            if(count>=COUNT)
            {
                exit(0);
            }
	    }
    }
    else
    {
        sleep(5);    
        kill(pid,SIGKILL);
        wait(NULL);
    }

	return NULL;
}

void set_interface_ip(const char *interface_name, struct my_iph *snd_iph)
{
	struct ifaddrs *ifaddr, *ifa;
	char host[NI_MAXHOST];

	/* Get all interfaces addresses */
	if (getifaddrs(&ifaddr) == -1) {
		perror_exit("getifaddrs");
	}

	/* Walk the linked list of interface addresses to get the specified interface's address */
	for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
		if (ifa->ifa_addr == NULL) {
			continue;
		}

		/* Address to name translation */	 
		int s = -1; 
		s = getnameinfo(ifa->ifa_addr, 
				sizeof(struct sockaddr_in),
				host, 
				NI_MAXHOST, 
				NULL, 
				0, 
				NI_NUMERICHOST);

		/* Get only the specified interface's name */
		if((strcmp(ifa->ifa_name, interface_name)==0)&&(ifa->ifa_addr->sa_family==AF_INET)) {
			if (s != 0) {
				perror_exit("getnameinfo()");
			}
			
			if (INTERFACE_PRINTED == false) {
				INTERFACE_PRINTED = true;
			}

			/* Set the source IP address of the given interface */
			struct sockaddr_in sa;
			inet_pton(AF_INET, host, &(sa.sin_addr));
			snd_iph->src_addr = sa.sin_addr.s_addr;
		}
	}	
}

void set_dest_ip(struct my_iph *snd_iph) 
{
	struct addrinfo hints;
	struct addrinfo *dest_info, *p;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;    		/* Get only IPv4 addresses */ 
	hints.ai_socktype = SOCK_STREAM;

	/* __Destination_resolution__ */
	int status = getaddrinfo(dest_host_name, NULL, &hints, &dest_info);
	if (status != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
		exit(EXIT_FAILURE);
	}

    
	/* Loop through the results and use the first (dest addr) we can */
	for(p = dest_info; p != NULL; p = p->ai_next) {
		/* Get the pointer to the address itself */
		if (p->ai_family == AF_INET) { 
			struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
			
			/* Set the destination IP address */
			snd_iph->dst_addr = ipv4->sin_addr.s_addr;
			
			break;
		}
	}
	free(dest_info);
}


void set_ip_hdr(struct my_iph *snd_iph)
{
	snd_iph->version = 4;			/* IPv4 */
	snd_iph->hdr_len = 5;			/* (5 * 4) = 20 bytes; no options */
	
	snd_iph->tos_precedence = 0x0;		// /* priority */
	snd_iph->tos_throughput = 0x0;		// /* low delay */
	snd_iph->tos_reliability = 0x0;		// /* high priority */
	snd_iph->tos_reserved = 0x0;

	snd_iph->tot_len = sizeof(struct my_iph) + sizeof(struct my_tcph);	/* No payload */
	snd_iph->identification = htons(54321); // test value;
	
	snd_iph->flg_rsrvd = 0x0;
	snd_iph->flg_DF = 0x0;
	snd_iph->flg_MF = 0x0;

	snd_iph->frg_offset = 0; 			/* Don't fragment */
	snd_iph->ttl = 255;				/* Max TTL */
	snd_iph->protocol = IPPROTO_TCP;	
	snd_iph->hdr_chk_sum = 0;			/* calc later */
	
	set_interface_ip("ens33", snd_iph);
	
	if (TARGET_RESOLVED == false) {
		set_dest_ip(snd_iph);
		TARGET_RESOLVED = true;
	}
}

void set_tcp_hdr(struct my_tcph *snd_tcph)
{
	snd_tcph->src_port = htons(atoi(COMMS_PORT));	
	snd_tcph->dst_port = htons(0);		/* Iterate through later */

	snd_tcph->seq_no = 0;	
	snd_tcph->ack_no = 0;			/* TCP ack is 0 in first packet */
	
	snd_tcph->rsvrd1 = 0x0;
	snd_tcph->data_offset = 5;		/* Assuming the minimal header length */

	snd_tcph->syn = 1;
	snd_tcph->urg = 0;
	snd_tcph->psh = 0;
	snd_tcph->rst = 0;
	snd_tcph->fin = 0;
	snd_tcph->ack = 0;

	snd_tcph->window = htons(5840);  
	snd_tcph->chksum = 0;			/* Will compute after header is completly set */
	
	snd_tcph->urg_ptr = 0; 
}
void set_raw_socket(void)
{
	g_sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
	if (g_sockfd == -1) {
		perror_exit("[#] Unable to create socket\n");
	} 
}

void set_socket_options(void)
{
	/* Tell the system that we are providing the IP header */
	if (setsockopt(g_sockfd, IPPROTO_IP, IP_HDRINCL, &(int){1}, sizeof(int)) < 0) {
		perror_exit("[#] Unable to set socket option\n");
	} 

	if (setsockopt(g_sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0){
		perror_exit("[#] setsockopt(SO_REUSEADDR) failed");
	} 
}

void create_thread(enum threadType type,pthread_t* i)
{
	int ret_v = -1;

	/* create a thread based on the type requested */
	switch (type)
	{
	case LISTENER_THREAD:
		ret_v = pthread_create(i, NULL, listener, NULL);
		break;

	case SCANNER_THREAD:
		ret_v = pthread_create(i, NULL, scanner, NULL);
		break;

	default:
		printf("[#] Invalid thread type has been requested for creation\n");
		return;
	}

	if (ret_v == -1) {
		printf("pthread_create failed: %d\n", type);
		exit(EXIT_FAILURE);
	}
}



