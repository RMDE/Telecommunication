/*****************************************************************************
** Author : Ye Xi
** ID     : 161720110
** Class  : 1617204
** Github : https://github.com/RMDE/Telecommunication/Expr.1/Scanner/tcpsyn.c
*****************************************************************************/
#include<stdio.h>
#include "tcpsyn.h"
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>

Host host[MAX_LAN_NUM];
int hostsum=0;
void scan_tcp_ports(char*);

void GetIP(){
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
        strcpy(host[hostsum].name,name);
        strcpy(host[hostsum].ip,ip);
        hostsum++;
    }
    fclose(hostname);
    fclose(hostip);
}

void syn()
{
    strcpy(host[hostsum].name,"localhost");
	strcpy(host[hostsum++].ip,"127.0.0.1");
	GetIP();
	for(int i=0;i<hostsum;i++)
	{
        printf("Now scan: %s\n",host[i].ip);
		scan_tcp_ports(host[i].ip);
	    // //pthread_join(g_listener_thread, NULL);
	    // //pthread_join(g_scanner_thread, NULL);
        // printf("end\n");
        // //printf("Now scan: %s\n",host[2].ip);
		// scan_tcp_ports(host[2].ip);

	}
}
