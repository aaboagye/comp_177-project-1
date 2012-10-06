// Name & E-mail: Celena Tan 			tan.celena@gmail.com
// Name & E-mail: Aseda Aboagye			aseda.aboagye@gmail.com
// File Name: client.h
// Course: COMP 177 Computer Networking
// Project: Pizza Consumers Union
// Created on: October 4, 2012
// Last Edited: October 4, 2012

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#ifndef CLIENT_H_
#define CLIENT_H_

typedef struct {
	char name1;
	char name2;
	int diameter1;
	int diameter2;
	int price1;
	int price2;
} Parlor;

typedef struct {
	int cost1;
	int cost2;
} Server_rtrn;

#endif






















