#/*****************************************************************************
#** Author : Ye Xi
#** ID     : 161720110
#** Class  : 1617204
#** Github : https://github.com/RMDE/Telecommunication/Expr.1/Scanner/Makefile
#*****************************************************************************/




CC = gcc
CFLAGS = -Wall -g
OBJS = main.o connection.o syn.o tcpsyn.o

scanner:$(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o scanner -pthread


main.o:main.c connection.h global.h tcpsyn.h
	$(CC) $(CFLAGS) -c main.c

connection.o:connection.c connection.h
	$(CC) $(CFLAGS) -c connection.c -pthread

tcpsyn.o:tcpsyn.c tcpsyn.h
	$(CC) $(CFLAGS) -c tcpsyn.c

syn.o:syn.c syn.h  
	$(CC) $(CFLAGS) -c syn.c -pthread

clean:
	-rm scanner
	-rm $(OBJS)

all:scanner
