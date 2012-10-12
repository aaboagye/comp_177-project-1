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

//Aseda's IP Addr v 4: 10.6.146.224
//#define PORT 4000//The port that the client will be connecting to
#define MAXDATASIZE 1024 //Max number of bytes we can get at once

/* Sites that might help us:
 * - http://cboard.cprogramming.com/networking-device-communication/98174-c-program-client-server.html
 * - ^^ASEDA!!! This might help you with the server, it has a sample program for client and server
 */

int main(int argc, char *argv[]) { //0 is name of program, 1 is the IP and, 2 port
	int status;
	char temp1[255], temp2[255];
	int sockfd = 0, numbytes;
	//long addr;
	char buffer[MAXDATASIZE];

	//struct hostent *he;
	//Connector's address information needed
	//struct sockaddr_in their_addr;
	struct addrinfo serv_addr;
	struct addrinfo *servinfo;

	Parlor info; //Needed to store input from the user
	Server_rtrn results; //Needed to store results from the server

	if(argc != 3) { //If no command line argument supplied
		fprintf(stderr, "Client-Usage: %s the_client_hostname\n", argv[0]);
		exit(1); //Just exit
	}

	memset(&info, 0, sizeof info); //Make sure the structure is empty
	memset(buffer, '0',sizeof(buffer));
	memset(&serv_addr, 0, sizeof serv_addr); //Zero the rest of the struct

	printf("\nWelcome to the Pizza program!\n\n"); //Welcome! =)

	printf("Enter the name of pizza parlor 1: "); //Asks info about first parlor
	scanf("%255s", temp1);
	info.name1_Len = strlen(temp1);
	info.name1 = temp1[0];
	printf("\nEnter the diameter of pizza 1 in inches: ");
	scanf("%hu.%hu", &info.inches1, &info.fracInches1);
	printf("\nEnter the price of pizza 1: $");
	scanf("%hu.%02hu", &info.dollar1, &info.cent1);

	printf("\nEnter the name of pizza parlor 2: "); //Asks info about the second parlor
	scanf("%255s", temp2);
	info.name2_Len = strlen(temp2);
	info.name2 = temp2[0];
	printf("\nEnter the diameter of pizza 2 in inches: ");
	scanf("%hu.%hu", &info.inches2, &info.fracInches2);
	printf("\nEnter the price of pizza 2: $");
	scanf("%hu.%02hu", &info.dollar2, &info.cent2);

	//Creating a socket
	//he = gethostbyname(argv[1]); // --> deprecated
	//he = gethostbyaddr((char *) argv[1], sizeof(argv[1]), AF_INET); //argv[1] = Asedas IP

	//Get the Host Information
	/*if(he == NULL) {
		perror("gethostbyaddr()");
		exit(1);
	}

	else {
		printf("Client - The remote host is: %s \n", argv[1]);
	}*/

	serv_addr.ai_family = AF_UNSPEC;
	serv_addr.ai_socktype = SOCK_STREAM; // TCP stream sockets
	serv_addr.ai_flags = AI_CANONNAME;     // fill in my IP for me

	if ((status = getaddrinfo(argv[1], argv[2], &serv_addr, &servinfo)) != 0) {
	    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
	    exit(1);
	}

	else {
		printf("\nClient - The getaddrinfo() is OK.. \n");
	}

	if((sockfd = socket(servinfo -> ai_family, servinfo -> ai_socktype, servinfo -> ai_protocol)) < 0) { //Print out failed if socket cannot be created
		printf("ERROR: Could not create socket!\n");
		exit(1);
	}

	else {
		printf("Client - The socket() sockfd is OK.. \n");
	}

	bind(sockfd, servinfo -> ai_addr, servinfo -> ai_addrlen);

	//their_addr.sin_family = AF_INET; //Host byte order

	//Short, network byte order

	printf("Server - Using %s and port %s...\n", argv[1], argv[2]);

	//Connecting the server...
	if(connect(sockfd, servinfo->ai_addr, servinfo->ai_addrlen)) {//connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) == -1) {
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
	//STILL NEED TO CODE THIS PART!
	//		sendto() == -1 then fail
	//		else printf("Client - The sendto() is OK.. \n");

	int len = strlen(buffer);

	int total_data = sizeof(Parlor) - 1 - 1 + info.name1_Len + 1 +info.name2_Len + 1;

	char* pointer_to_buffer = malloc(total_data);

	Parlor* pointer_to_buffer_as_struct;  // No structure here - just an empty pointer!

	pointer_to_buffer_as_struct = (Parlor *) pointer_to_buffer; // Casting

	pointer_to_buffer_as_struct -> dollar1 = htons(info.dollar1);
	pointer_to_buffer_as_struct -> dollar2 = htons(info.dollar2);
	pointer_to_buffer_as_struct -> cent1 = htons(info.cent1);
	pointer_to_buffer_as_struct -> cent2 = htons(info.cent2);
	pointer_to_buffer_as_struct -> inches1 = htons(info.inches1);
	pointer_to_buffer_as_struct -> inches2 = htons(info.inches2);
	pointer_to_buffer_as_struct -> fracInches1 = htons(info.fracInches1);
	pointer_to_buffer_as_struct -> fracInches2 = htons(info.fracInches2);
	pointer_to_buffer_as_struct -> name1_Len = htons(info.name1_Len);
	pointer_to_buffer_as_struct -> name2_Len = htons(info.name2_Len);

	strcpy(&pointer_to_buffer_as_struct -> name1, temp1);
	strcpy(&pointer_to_buffer_as_struct -> name2, temp2);

	if(send(sockfd, (char *)pointer_to_buffer_as_struct, len, 0) == -1) {
		perror("send()");
		exit(1);
	}

	else {
		printf("Client - The send() is OK.. \n");
	}

	//Read and Write via socket???
	//rc = write(sockfd, &ch, 1);
	//read(sockfd, &ch, 1);
	//recv() difference with recvfrom()???

	//Receiving reply... Read back from server
	if((numbytes = recv(sockfd, buffer, MAXDATASIZE - 1, 0)) == -1) {
		perror("recv()");
		exit(1);
	}

	else {
		printf("Client - The recv() is OK.. \n");
	}

	//Get results and store into Server_rtrn structure
	buffer[numbytes] = '\0';
	printf("Client - Received: %s ...\n", buffer);
	//results.cost1 = ;
	//results.cost2 = ;
	//results.winner = ;

	printf("\n\nThe cost per square inch of pizza 1 is: $%hu.%hu", results.dollar1, results.cent1);
	printf("\nThe cost per square inch of pizza 2 is: $%hu.%hu", results.dollar2, results.cent2);

	printf("\n%s offers the most economical choice. \n\nEnjoy!", results.winner);
	/*
	if(results.dollar1 < results.cost2) { //If parlor 1 cost is less than parlor 2 cost
		printf("%s offers the most economical choice. \n\nEnjoy!", &info.name1);
	}

	if(results.cost1 > results.cost2) { //If parlor 2 cost is less than parlor 1 cost
		printf("%s offers the most economical choice. \n\nEnjoy!", &info.name2);
	}

	if(results.cost1 == results.cost2) { //If both cost are equal
		printf("Both parlor offers the most economical choice. \n\nEnjoy!");
	}*/

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

	free(pointer_to_buffer_as_struct);
	freeaddrinfo(servinfo);
	printf("\nClient - Closing sockfd.. \n");
	close(sockfd); //Close socket when finished
	return 0;
}

