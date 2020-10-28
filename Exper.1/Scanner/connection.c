/*****************************************************************************
** Author   : Ye Xi
** ID       : 161720110
** Class    : 1617204
** Github   : https://github.com/RMDE/Telecommunication/Expr.1/Scanner/connection.c 
** function : using connect function of socket to scanner
*****************************************************************************/
#include <sys/socket.h>
#include <stdbool.h>
#include <netinet/in.h>
#include <errno.h>
#include <netdb.h>
#include <signal.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/select.h>
#include <pthread.h>
#define MAX_LAN_NUM 250
#define DEFAULT_THREAD_NUM 10
#define START_PORT 1
#define END_PORT 1024
//struct of host
typedef struct{
	char name[40];
	char ip[40];
}Host;
//struct of scan thread args
typedef struct{
	int num;
	char *ip;
	int port;
	int end;
}ScanThreadArgs;

//store the infomation od host in lan
Host hosts[MAX_LAN_NUM];
int hostsNum = 0;

void GetIp(){
	FILE *hostname,*hostip;
	char name[40],ip[40];
	//store the every host's name and ip in the same lan into file
	system("sudo arp -a | cut -d \" \" -f 1 > .hname");
	system("sudo arp -a | cut -d \" \" -f 2 | sed \"s/\(//g\" | sed \"s/)//g\" > .hip");
	//get host's name and ip
	hostname = fopen("./.hname","r");
	hostip = fopen("./.hip","r");
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
	fclose(hostname);
	fclose(hostip);
	//output the infomation of hosts in Lan
	printf("All hosts in LAN:\n");
	printf("No\tName\t\tIP\n");
	for(int i=0;i<hostsNum;i++)
	{
		printf("%d\t%-16s%s\n",i+1,hosts[i].name,hosts[i].ip);
    }

}

//port scanner
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
		if(errno == EINPROGRESS)
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

//scaning thread function
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
