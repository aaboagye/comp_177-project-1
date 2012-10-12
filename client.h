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
	int dollar1;
	int cent1;
	int inches1;
	int fracInches1;

	int dollar2;
	int cent2;
	int inches2;
	int fracInches2;

	int name1_Len;
	int name2_Len;

	char name1;
	char name2;
}__attribute__ ((packed)) Parlor ;

typedef struct {
	int dollar1;
	int dollar2;
	int cent1;
	int cent2;
	char winner;
}__attribute__ ((packed)) Server_rtrn ;

#endif






















