// Name & E-mail: Celena Tan 			tan.celena@gmail.com
// Name & E-mail: Aseda Aboagye			aseda.aboagye@gmail.com
// File Name: main.c
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
#include "client.h"

//NOTE: Hi Aseda! Hope you're enjoying your 3 day weekend! =)

/* Sites that might help us:
 * - http://cboard.cprogramming.com/networking-device-communication/98174-c-program-client-server.html
 * - ^^ASEDA!!! This might help you with the server, it has a sample program for client and server
 */

int main(int argc, char *argv[]) {
	int status, answer;
	int sockfd = 0;
	char recvBuffer[1024];

	Parlor info; //Needed to store input from the user
	Server_rtrn results; //Needed to store results from the server

	memset(&info, 0, sizeof info); //Make sure the structure is empty
	memset(recvBuffer, '0',sizeof(recvBuffer));

	printf("Welcome to the Pizza program!\n\n"); //Welcome! =)

	printf("Enter the name of pizza parlor 1: "); //Asks info about first parlor
	scanf("%s", &info.name1);
	printf("\nEnter the diameter of pizza 1 in inches: ");
	scanf("%i", &info.diameter1);
	printf("\nEnter the price of pizza 1: ");
	scanf("%d", &info.price1);

	printf("\n\nEnter the name of pizza parlor 2: "); //Asks info about the second parlor
	scanf("%s", &info.name2);
	printf("\nEnter the diameter of pizza 2 in inches: ");
	scanf("%i", &info.diameter2);
	printf("\nEnter the price of pizza 2: ");
	scanf("%d", &info.price2);

	//Creating a socket
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) { //Print out failed if socket cannot be created
		printf("\nERROR: Could not create socket!");
		return 1;
	}

	//Connecting the server... // printf(”An error has occurred: %s\n", strerror(errno));
	//sockfd = socket(AF_INET, SOCK_STREAM, 0);		//Create socket for client.
	//server <server port> //To invoke the server
	//answer = connect(sockfd, (struct sockaddr *)&address, len);

	//send(); Send data to the server
	//recv(); Receive data from the server

	/*
	if(result == -1) {
        perror("Error has occurred");
        exit(-1);
    }
	*/

	//Sending information...
	//Receiving reply...

	//Read and Write via socket??? CELENA RESEARCH MORE!!!
	//rc = write(sockfd, &ch, 1);
	//read(sockfd, &ch, 1);

	//Get results and store into Server_rtrn structure

	printf("The cost per square inch of pizza 1 is: %i", results.cost1);
	printf("The cost per square inch of pizza 2 is: %i", results.cost2);

	if(results.cost1 < results.cost2) { //If parlor 1 cost is less than parlor 2 cost
		printf("%s offers the most economical choice. \n\nEnjoy!", &info.name1);
	}

	if(results.cost1 > results.cost2) { //If parlor 2 cost is less than parlor 1 cost
		printf("%s offers the most economical choice. \n\nEnjoy!", &info.name2);
	}

	if(results.cost1 == results.cost2) { //If both cost are equal
		printf("Both parlor offers the most economical choice. \n\nEnjoy!");
	}

	//Research SIGINT!!!!
	//Your server should "capture" the user's keystroke (technically, the SIGINT interrupt triggered by CTRL-C),
	//Call a function that properly shuts down and cleans up the server by closing the sockets, & then exit gracefully
	//Ask Aseda if this should be implemented here or in the server part of the project
	//The prototype of the signal function ----> void (*signal(int signo, void (*func )(int)))(int);
	//This site was useful for SIGINT:    http://www.thegeekstuff.com/2012/03/catch-signals-sample-c-code/
	/*
	void sig_handler(int signo) {
		if (signo == SIGINT)	printf("received SIGINT\n");
	}

	int main(void) {
  	  	  if (signal(SIGINT, sig_handler) == SIG_ERR)
  	  	  	  printf("\ncan't catch SIGINT\n"); //A long long wait so that we can easily issue a signal to this process
  	  	  while(1)
    		sleep(1);
  	  	  return 0;
	} */

	close(sockfd); //Close socket when finished
	return 0;
}
