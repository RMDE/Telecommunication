#include<stdlib.h>
#include<sys/time.h>
#include<stdio.h>
#include "connection.h"
#include "global.h"
#include "tcpsyn.h"
int main()
{
    struct timeval start,end;
	printf("*****************The first method: TCP-connect********************\n");
    gettimeofday(&start,NULL);
	connection();
    gettimeofday(&end,NULL);
    double total_time=(end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec);
    total_time/=1000000;
	printf("***************** End of function: %.2f seconds****************\n",total_time);
    printf("*******************The second method: TCP-SYN******************\n");
    gettimeofday(&start,NULL);
    syn();
    gettimeofday(&end,NULL);
    total_time=(end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec);
    total_time/=1000000;
	printf("***************** End of function: %.2f seconds****************\n",total_time);
	return 0;
}
