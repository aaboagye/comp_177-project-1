// Name & E-mail: Celena Tan 			tan.celena@gmail.com
// Name & E-mail: Aseda Aboagye			aseda.aboagye@gmail.com
// File Name: main.c
// Course: COMP 177 Computer Networking
// Project: Pizza Consumers Union
// Created on: October 4, 2012
// Last Edited: October 17, 2012

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
#define MAXDATASIZE 1024 //Max number of bytes we can get at once

	//server <server ip> <server port> //To invoke the server
int main(int argc, char *argv[]) { //0 is name of program, 1 is the IP and, 2 port
	int status;
	char winner[255];
	char temp1[255], temp2[255];
	int sockfd = 0, numbytes;
	char buffer[MAXDATASIZE];
	Parlor info;
	Server_rtrn results; //Needed to store results from the server

	//struct sockaddr_in their_addr;
	struct addrinfo serv_addr;
	struct addrinfo *servinfo;

	if(argc != 3) { //If no command line argument supplied
		fprintf(stderr, "Client-Usage: %s <server ip> <server port>\n", argv[0]);
		exit(1); //Just exit
	}

	memset(&info, 0, sizeof info); //Make sure the structure is empty
	memset(buffer, '0', sizeof buffer);
	memset(&serv_addr, 0, sizeof serv_addr); //Zero the rest of the struct

	printf("\nWelcome to the Pizza Program!\n\n"); //Welcome! =)

	printf("Enter the name of Pizza Parlor 1: "); //Asks info about first parlor
	scanf("%255s", temp1);
	info.name1_Len = strlen(temp1);
	printf("\nEnter the diameter of pizza 1 in inches: ");
	scanf("%hu.%hu", &info.inches1, &info.fracInches1);
	printf("\nEnter the price of pizza 1: $");
	scanf("%hu.%02hu", &info.dollar1, &info.cent1);

	printf("\nEnter the name of Pizza Parlor 2: "); //Asks info about the second parlor
	scanf("%255s", temp2);
	info.name2_Len = strlen(temp2);
	printf("\nEnter the diameter of pizza 2 in inches: ");
	scanf("%hu.%hu", &info.inches2, &info.fracInches2);
	printf("\nEnter the price of pizza 2: $");
	scanf("%hu.%02hu", &info.dollar2, &info.cent2);

	Parlor* send_data = ((Parlor *) malloc(sizeof(Parlor) + info.name1_Len + info.name2_Len));
	memset(send_data, 0, sizeof(send_data));

	send_data -> dollar1 = htons(info.dollar1);
	send_data -> dollar2 = htons(info.dollar2);
	send_data -> cent1 = htons(info.cent1);
	send_data -> cent2 = htons(info.cent2);
	send_data -> inches1 = htons(info.inches1);
	send_data -> inches2 = htons(info.inches2);
	send_data -> fracInches1 = htons(info.fracInches1);
	send_data -> fracInches2 = htons(info.fracInches2);
	send_data -> name1_Len = htons(info.name1_Len);
	send_data -> name2_Len = htons(info.name2_Len);

	strncpy((char *)(send_data + 1), temp1, strlen(temp1));
	strncpy(((char *)(send_data + 1) + (info.name1_Len + 1)), temp2, strlen(temp2));

	serv_addr.ai_family = AF_UNSPEC;
	serv_addr.ai_socktype = SOCK_STREAM; //TCP stream sockets
	serv_addr.ai_flags = AI_CANONNAME;     //Fill in my IP for me

	// Get info
	if ((status = getaddrinfo(argv[1], argv[2], &serv_addr, &servinfo)) != 0) {
	    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status)); //GAI
	    exit(1);
	}

	else {
		printf("\nClient - The getaddrinfo() is OK.. \n");
	}

	//Create socket. Print out failed if socket cannot be created
	if((sockfd = socket(servinfo -> ai_family, servinfo -> ai_socktype, servinfo -> ai_protocol)) < 0) {
		printf("ERROR: Could not create socket!\n");
		exit(1);
	}

	else {
		printf("Client - The socket() sockfd is OK.. \n");
	}

	bind(sockfd, servinfo -> ai_addr, servinfo -> ai_addrlen);
	printf("Server - Using %s and port %s...\n", argv[1], argv[2]);

	//Connecting the server...
	if(connect(sockfd, servinfo -> ai_addr, servinfo -> ai_addrlen)) { //connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) == -1) {
		perror("Connect()");
		exit(1);
	}

	else {
		printf("Client - The connect is OK.. \n");
	}

	//Send the Parlor struct and two parlor names over
	if(send(sockfd, (char *)send_data, sizeof(Parlor) + strlen(temp1) + 1 + strlen(temp2) + 1, 0) == -1) {
		perror("send()");
		exit(1);
	}

	else {
		printf("Client - The send() is OK.. \n");
	}

	//Receiving reply... Read back from server
	if((numbytes = recv(sockfd, buffer, sizeof(buffer), 0)) == -1) {
		perror("recv()");
		exit(1);
	}

	else {
		printf("Client - The recv() is OK.. \n");
	}

	//After receiving from buffer

	Server_rtrn *recv_data = (Server_rtrn *)buffer;
	results.dollar1 = ntohs(recv_data->dollar1); //Store data
	results.dollar2 = ntohs(recv_data->dollar2);
	results.cent1 = ntohs(recv_data->cent1);
	results.cent2 = ntohs(recv_data->cent2);
	results.winner_Len = ntohs(recv_data->winner_Len);
	strncpy(winner, ((char *)(recv_data) + sizeof(Server_rtrn)), results.winner_Len + 1);

	//Get results and store into Server_rtrn structure
	printf("Client - Received: %s ...\n", buffer);
	printf("\n\nThe cost per square inch of pizza 1 is: $%hu.%02hu", results.dollar1, results.cent1);
	printf("\nThe cost per square inch of pizza 2 is: $%hu.%02hu", results.dollar2, results.cent2);
	printf("\n%s offers the most economical choice. \n\nEnjoy!", winner);
	free(send_data);
	freeaddrinfo(servinfo);
	printf("\nClient - Closing sockfd.. \n");
	close(sockfd); //Close socket when finished
	return 0;
}
