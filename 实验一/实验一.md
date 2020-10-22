## 目录

[TOC]

------

## SCANNER 简单网络扫描程序

### 实验目的

- 熟悉并实现网络扫描的基本原理
- 了解网络扫描的几种常用方法

### 实验环境

- Linux

### 实验语言

- c/c++

### 实验内容

- 编写一个扫描局域网内主机的程序。要求可以显示局域网内的主机名列表，IP地址列表，并可以显示哪些主机开放了哪些端口。

### 实验理论

- 监听状态：服务端程序等待客户端的连接，即端口处于开放状态

- 查看端口监听情况

  ```bash
  Windows：
  	netstat
  Linux:
  	netstat -an  /  lsof -i
  ```

1. #### TCP connect

   - 原理：利用`socket`中的`connect()`函数向目标`IP+port`发送连接请求，若成功，说明端口是开放的

   - 优点：比较可靠，因为TCP是可靠协议，当丢包时会重传SYN帧

   - 缺点：当端口不存在时，源主机会不断尝试发SYN帧企图得到SYN答应，多次尝试后才放弃，因此造成扫描时间较长，且容易被目标主机发现。

   - 改进：针对扫描时间长问题，可通过多线程同时运行、重设超时时间来改进

     - 多线程

       ```c
       //创建线程
       pthread_create(&thrd1,NULL,(void*)&thread_function,(void*)&some_argument);
       //线程等待
       pthread_join(thrd1,NULL);
       //线程结束
       pthread_exit(void* retval);
       ```

     - 设置超时时间

       （1）设置`socket`为非阻塞方式

       ```c
       int fcntl(int fd,int cmd,.../*arg*/);
       int ioctl(int fd,unsigned long request,.../*arg*/);
       ```

       （2）超时等待

       ```c
       //同步I/O多路复用
       int select(int nfds,fd_set *readfds,fd_set *writefds,fd_set *exceptfds,struct timeval *timeout);
       //nfds：文件描述符
       //readfds：可读文件描述符词组
       //writefds：可写文件描述符词组
       //exceptfds：例外文件描述符词组
       //timeout：等待时间
       //相关函数
       void FD_ZERO(fd_set *set);//清除描述符词组set的全部位
       void FD_CLR(int fd,fd_set *set);//清除描述符词组set中相关fd的位
       void FD_SET(int fd,fd_set *set);//设置描述符词组set中相关fd的位
       int FD_ISSET(int fd,fd_set *set);//测试描述符词组set中相关fd的位是否为真
       ```

       （3）获取`socket`状态

       ```c
       int getsockopt(int sockfd,int level,int optname,void *optval,socklen_t *optlen);
       //sockfd：socket的文件描述符
       //level：操作的网络层，一般为SOL_SOCKET
       //optname：操作的选项，如SO_DEBUG等
       //optval：保存结果的内存地址
       //optlen：optval空间的大小
       ```

   - 相关函数：

     ```c
     int socket(int family, int type , int protocol)
         //family:AF_INET, AF_UNIX, AF_NS
         //type:SOCK_STREAM(TCP), SOCK_DGRAM(UDP), SOCK_RAW(IP)
         //protocol:IPPROTO_TCP, IPPTOTO_UDP
     int connect (int sockfd, struct sockaddr* serv_addr, int addrlen);
     //sockfd: 已创建的socket标识符
     //serv_addr: 目标服务器的ip+port地址信息
     //addrlen: serv_addr的长度
     ```

2. #### TCP SYN

   - 原理：向目标端口发送SYN数据包，若收到SYN+ACK，则端口开放，收到RST说明不开放。SYN扫描不会完成TCP三次握手过程，又称半开放扫描。
   - 优点：速度快，如果不被防火墙过滤的话，基本能收到答应包
   - 缺点：扫描易被发现；自己发数据包，在IP层，不可靠，会丢包；实现比TCP connect方法较复杂
   - 过程：
     - 创建SOCKET_RAW套接字，即在网络层进行编程
     - 给创建好的RAW SOCKET，通过`setsockopt`设置`IP_HDPINCL`属性，即在数据链路层进行编程，使可自定义IP头
     - 发送构造好的SYN数据包
     - 创建好另一个SOCKET_RAW，设置好`IP_HDPINCL`，绑定到本地IP的任意端口。使用`WsAloctl()`设置`SIO_RCVALL`属性，使套接字可接受通过本地IP的所有数据包
     - 判断抓到的包的`SRC IP`和`SRC PORT`，ACK是否等于发送出去的，包的`Identificant+1`，标志位是否为SYN`(0X0012)`即可
   - 在Windows下的版本变更（XP --> Win10)带来的限制
     - 创建`SOCKET_RAW`必须管理员权限
     - 不能使用`SOCKET_RAW`发送TCP数据包，否则丢弃
     - 不能使用`SOCKET_RAW`发送源IP与本地网卡IP不一致的UDP数据包
     - 导致该方法在Windows下不可行

3. #### TCP FIN

   - 原理：根据四报文握手过程，主动结束的一方会发送FIN帧。当发送一个FIN帧给非监听窗口，会有RST应答；反之，发送给正在监听的窗口，则无任何反应
   - 优点：隐蔽性好，速度块
   - 缺点：只可应用于Linux，Windows下无论端口是否监听都会返回RST帧；不可靠，无法判别是无应答还是丢包了

4. #### UDP ICMP

   - 原理：向端口发送UDP报文，若端口开放则无响应；若不开放，则回复ICMP端口不可达报文
   - 优点：Linux和Windows系统皆可使用
   - 缺点：不可靠传输；返回的是错误信息，速度相较于TCP的FIN、SYN扫描更慢；若发送UDP包过快，回应的ICMP包会出现大量丢失的现象

### 实验实践

- 直接使用已有工具`nmap`来进行实验观察

  ![1585826346195](C:\Users\猫猫\AppData\Roaming\Typora\typora-user-images\1585826346195.png)

1. #### TCP connect

   - 查找所在局域网内所有主机的IP地址

     ```c
     //1.将局域网内主机名存入.hname文件，IP地址存入.hip文件
     system("sudo arp -a | cut -d \" \" -f 1 > .hname");
     system("sudo arp -a | cut -d \" \" -f 2 | sed \"s/\(//g\" | sed \"s/)//g\" > .hip");
     //2.将主机信息从文件存入结构体hosts中
     while(fgets(name,40,hostname)&&fgets(ip,40,hostip))
     {
     	if(name[strlen(name)-1] == '\n')
     		name[strlen(name)-1] = '\0';
     	if(ip[strlen(ip)-1] == '\n')
     		ip[strlen(ip)-1] = '\0';
     	//store the infomation into struct Host
     	strcpy(hosts[hostsNum].name,name);
         strcpy(hosts[hostsNum].ip,ip);
         hostsNum++;
     }
     //3.将本机信息也存入其中
     strcpy(hosts[hostsNum].name,"localhost");
     strcpy(hosts[hostsNum++].ip,"127.0.0.1");
     ```

   - 对每一个主机的0~1024端口进行`connect()`连接

     ```c
     bool tcpConnect(char *ip,int port){
             struct sockaddr_in address;
             struct timeval timeout;
             fd_set fdr,fdw;
             //initialize
             memset(&address,0,sizeof(address));
             memset(&timeout,0,sizeof(timeout));
             //setting the address
             address.sin_family = AF_INET;
             address.sin_addr.s_addr = inet_addr(ip);
             address.sin_port = htons(port);
             //creating the socket
             int sn = socket(AF_INET,SOCK_STREAM,0);
             if(sn<0)
             {
                     printf("Socket create error!\n");
                     exit(0);
             }
             //设置套接字为非阻塞模式
             int fcntlStatus = fcntl(sn,F_GETFL,0);
             if(fcntlStatus<0)
             {
                     close(sn);
                     return false;
             }
             fcntlStatus |= O_NONBLOCK;
             if(fcntl(sn,F_SETFL,fcntlStatus)<0)
             {
                     close(sn);
                     return false;
             }
             //try to connect
             int res = connect(sn,(struct sockaddr *)&address,sizeof(address));
             if(res!=0)
             {
                     if(errno == EINPROGRESS)//if timeout
                     {
                             FD_ZERO(&fdr);
                             FD_ZERO(&fdw);
                             FD_SET(sn, &fdr);
                             FD_SET(sn, &fdw);
                             //setting timeout to 0.5s
                             timeout.tv_sec = 0.5;
                             timeout.tv_usec = 0;
                             res = select(sn + 1, &fdr, &fdw, NULL, &timeout);
                             //when connect correctly
                             if(res==1 && FD_ISSET(sn,&fdw))
                             {
                                     close(sn);
                                     return true;
                             }
                             close(sn);
                             return false;
                     }
             }
         return false;
     }
     ```

   - 进行多线程创建连接

     ```c
     //每个线程在确认好一个端口后，端口号加上当前进程数，开始确认下一个端口
     void *ThreadFunc(void* args){
             ScanThreadArgs* temp;
             temp = (ScanThreadArgs*)args;
             while(temp->port <= temp->end)
             {
                     if(tcpConnect(temp->ip,temp->port))
                             printf("\tport %d  --open\n",temp->port);
                     temp->port +=temp->num;//because there are "num" threads running now,every thread search one port
             }
     }
     //对给定的ip地址的0~1024端口进行确认
     int scan(char* ip){
             pthread_t *pthreads;
             ScanThreadArgs *args;
             pthreads = (pthread_t*)malloc(sizeof(pthread_t)*DEFAULT_THREAD_NUM);
             args = (ScanThreadArgs*)malloc(sizeof(ScanThreadArgs)*DEFAULT_THREAD_NUM);
             printf("Now scan %s:\n",ip);
             for(int i=0;i<DEFAULT_THREAD_NUM;i++)
             {
                     memset(&args[i],0,sizeof(args[i]));
                     args[i].num = DEFAULT_THREAD_NUM;
                     args[i].port = START_PORT+i;
                     args[i].ip = ip;
                     args[i].end = END_PORT;
                     if(pthread_create(&pthreads[i],NULL,ThreadFunc,(void*)&args[i])==-1)//create threads to run
                     {
                             printf("can't create pthread!\n");
                             return 0;
                     }
             }
             //sleep(1);
             for(int i=0;i<DEFAULT_THREAD_NUM;i++)//stop every thread
                     pthread_join(pthreads[i],NULL);
             free(pthreads);
             free(args);
             return 0;
     }
     //对局域网内所有的主机ip进行遍历
     void connection()
     {
             strcpy(hosts[hostsNum].name,"localhost");
             strcpy(hosts[hostsNum++].ip,"127.0.0.1");
             GetIp();
             for(int i=0;i<hostsNum;i++)
             {
                     scan(hosts[i].ip);
             }
     }
     ```

2. #### TCP SYN

   - 获取局域网中所有主机的IP地址（如上）

   - 设置TCP连接请求报文

     ```c
     //1.设置ip地址
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
     
     //2.设置IP头部
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
     //3.设置TCP头部
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
     //设置校验和
     uint16_t csum(const void *data, const int length)
     {
     	uint16_t *accumalator = (uint16_t *)data;
     	uint64_t sum = 0
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
     }
     ```

     

   - 对每个IP的`1~1024`端口发送TCP连接请求报文

     ```c
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
         printf("Now Scan %s\n",inet_ntoa(p_dest_addr.sin_addr));
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
     ```

   - 对收到的报文进行监听，判断是否是答复报文

     ```c
     void* listener(__attribute__((unused)) void *unused)
     {
         int count=0;
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
     		/* Get the pointers to the IP & TCP headers */
     		struct my_iph *recv_iph = (struct my_iph*)response_packet;
     		struct my_tcph *recv_tcph = (struct my_tcph*)(response_packet + 4 * (recv_iph->hdr_len));
     		/* Check if the message is for COMMS_PORT port */
     		if (recv_tcph->dst_port != ntohs(atoi(COMMS_PORT))) {
     			continue;
     		}
             count++;
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
     			if (discovered_ports[ntohs(recv_tcph->src_port) == 1]) {
     				continue;
     			}
     			printf("[*] Port: %d is open.\n", ntohs(recv_tcph->src_port));
     			/* Close the connection */
     			discovered_ports[ntohs(recv_tcph->src_port)] = 1;
     			close_connection(ntohs(recv_tcph->src_port), from_addr);
     		}
             if(count>=COUNT)
                 break;
     	}
     	return NULL;
     }
     ```

   - 将发送报文与监听报文作为双线程

     ```c
     void scan_tcp_ports(char* ip)
     {
     	pthread_t g_listener_thread,g_scanner_thread;
         dest_host_name = ip;
     	set_raw_socket();
     	set_socket_options();
     	create_thread(LISTENER_THREAD,&g_listener_thread);
     	create_thread(SCANNER_THREAD,&g_scanner_thread);
     	pthread_join(g_listener_thread, NULL);
     	pthread_join(g_scanner_thread, NULL);
     }
     ```

3. 实验结果

   ![1603344695239](C:\Users\猫猫\AppData\Roaming\Typora\typora-user-images\1603344695239.png)

   

------

## 总结

​	这次的实验基于connect（）的端口扫描器以及网络嗅探器完全是自己完成的，基于TCP SYN的端口扫描程序是参考github上已有项目，原因是自己对于如何进行`raw_socket`创建以及发送还不够了解，所以就找了现成代码学习，但是找到的代码有一些问题，它在监听收到的回复报文是一个死循环，我采用每收到一个相关回复报文就使计数器加1，当计数器达到一定值后便退出循环，从而使得程序可以接着对下一个IP进行检测。但是这样其实又面另一个问题，当目标主机不返回任何数据包时，计数器就一直无法增加，导致程序卡死。我的解决方案是将接受程序作为子进程进行，父进程进行计时，当超过5s还未收到任何数据包时，则强制杀死子进程结束该IP的端口扫描，进行下一个。

​	经过这次实验，我充分理解了端口扫描以及网络嗅探的原理，并掌握了不同的方法，同时我也学习到了如何进行多线程运行。对于在计网中学习的知识印象更深刻了，并首次将其进行了实践，成就感十足。

