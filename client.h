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
	uint16_t dollar1;
	uint16_t cent1;
	uint16_t inches1;
	uint16_t fracInches1;

	uint16_t dollar2;
	uint16_t cent2;
	uint16_t inches2;
	uint16_t fracInches2;

	uint16_t name1_Len;
	uint16_t name2_Len;

}__attribute__ ((packed)) Parlor ;

typedef struct {
	uint16_t dollar1;
	uint16_t dollar2;
	uint16_t cent1;
	uint16_t cent2;
	uint16_t winner_Len;
	char winner;
}__attribute__ ((packed)) Server_rtrn ;

#endif






















