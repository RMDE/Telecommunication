#ifndef _TCPSYN_H_
#define _TCPSYN_H_
#define MAX_LAN_NUM 255

typedef struct{
	char name[40];
	char ip[40];
}Host;


void syn();

#endif
