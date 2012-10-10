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
#include <netinet/in.h>
#include <netdb.h>
#include "client.h"

#define PORT 4000//The port that the client will be connecting to
#define MAXDATASIZE 1024 //Max number of bytes we can get at once

/* Sites that might help us:
 * - http://cboard.cprogramming.com/networking-device-communication/98174-c-program-client-server.html
 * - ^^ASEDA!!! This might help you with the server, it has a sample program for client and server
 */

int main(int argc, char *argv[]) {
	int status, answer;
	int sockfd = 0, numbytes;
	char buffer[MAXDATASIZE];

	struct hostent *he;
	//Connector's address information needed
	struct sockaddr_in their_addr;

	Parlor info; //Needed to store input from the user
	Server_rtrn results; //Needed to store results from the server

	if(argc != 2) { //If no command line argument supplied
		fprintf(stderr, "Client-Usage: %s the_client_hostname\n", argv[0]);
		exit1(1); //Just exit
	}

	memset(&info, 0, sizeof info); //Make sure the structure is empty
	memset(buffer, '0',sizeof(buffer));

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

	he = gethostbyname(argv[1]);
	//Get the Host Information
	if(he == NULL) {
		perror("gethostbyname()");
		exit(1);
	}

	else {
		printf("Client - The remote host is: %s \n", argv[1]);
	}

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) { //Print out failed if socket cannot be created
		printf("\nERROR: Could not create socket!");
		exit(1);
	}

	else {
		printf("Client - The socket() sockfd is OK.. \n");
	}

	their_addr.sin_family = AF_INET; //Host byte order
	//Short, network byte order
	printf("Server - Using %s and port %d...\n", argv[1], PORT); //Need to add port #
	their_addr.sin_port = htons(PORT);
	their_addr.sin_addr = *((struct in_addr *)he->h_addr);
	memset(&(their_addr.sin_zero), '\0', 8); //Zero the rest of the struct

	//Connecting the server...
	if(connect(sockfd, (struct sockaddr *)&their_addr, sizeof(struct sockaddr)) == -1) {
		perror("Connect()");
		exit(1);
	}

	else {
		printf("Client - The connect is OK.. \n");
	}

	//printf(”An error has occurred: %s\n", strerror(errno));

	//sockfd = socket(AF_INET, SOCK_STREAM, 0);		//Create socket for client.
	//fcntl(sockfd, F_SETFL, O_NONBLOCK);

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


	//Read and Write via socket??? CELENA RESEARCH MORE!!!
	//rc = write(sockfd, &ch, 1);
	//read(sockfd, &ch, 1);

	//Receiving reply...
	//Read back from server
	if((numbytes = recv(sockfd, buffer, MAXDATASIZE - 1, 0)) == -1) {
		perror("recv()");
		exit(1);
	}

	else {
		printf("Client - The recv() is OK.. \n");
	}

	//Get results and store into Server_rtrn structure
	buffer[numbytes] = '\0';
	printf("client - Received: %s", buffer);
	//results.cost1 = ;
	//results.cost2 = ;

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

	printf("Client - Closing sockfd\n");
	close(sockfd); //Close socket when finished
	return 0;
}
