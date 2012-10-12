#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "server.h"

#define QUEUE 10
#define MAX_BUF_SIZE 1024
#define MAX_NAME_SIZE 255

int main(int argc, char **argv){
	if(argc < 2){
		printf("Usage:	%s <server port>\n", argv[0]);
		return 1;
	}
	struct addrinfo hints;
	struct addrinfo *res;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	int status = getaddrinfo(NULL, argv[1], &hints, &res);
	if(status !=0){
		fprintf(stderr, "getaddrinfo ERROR: %s\n",gai_strerror(status));
		return 1;
	}
	int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if(sockfd<0){
		perror("server: socket");
		return 1;
	}
	status = bind(sockfd, res->ai_addr, res->ai_addrlen);
	if(status == -1){
		perror("server: bind");
		return 1;
	}
	status = listen(sockfd, QUEUE);
	if(status == -1){
		perror("server: listen");
		return 1;
	}
	int sockfd_client;
	struct sockaddr_storage their_addr;
	socklen_t addr_size = sizeof their_addr;
	sockfd_client = accept(sockfd, (struct sockaddr *) &their_addr, &addr_size);
	if(sockfd_client == -1){
		perror("server: accept");
		return 1;
	}
	/*	At this point, the socket is set up for the server and has accepted a
	 *	client's connect().
	 */
	char buffer[MAX_BUF_SIZE];
	memset(buffer, 0, MAX_BUF_SIZE);
	/* read in transmission from client. */
	/* Now I need to separate the data into the respective fields */
	char vendor1[MAX_NAME_SIZE];
	char vendor2[MAX_NAME_SIZE];
	int total_size1 = sizeof(struct parlor) -1 -1 + 256;
	char* buffer_ptr1 = malloc(total_size1);
	struct parlor* ptr_bytes1;
	ptr_bytes1 = (struct parlor*) buffer_ptr1;
	/* here there needs to be something along the lines of: */
	recv(sockfd_client, buffer, 160, 0); /* have received the whole buffer */
	ptr_bytes1 = (struct parlor*) buffer;
	ptr_bytes1->pizza1_dollars = ntohs(ptr_bytes1->pizza1_dollars);
	ptr_bytes1->pizza1_cents = ntohs(ptr_bytes1->pizza1_cents);
	ptr_bytes1->pizza1_inches = ntohs(ptr_bytes1->pizza1_inches);
	ptr_bytes1->pizza1_fractional_inches = ntohs(ptr_bytes1->pizza1_fractional_inches);
	ptr_bytes1->pizza2_dollars = ntohs(ptr_bytes1->pizza2_dollars);
	ptr_bytes1->pizza2_cents = ntohs(ptr_bytes1->pizza2_cents);
	ptr_bytes1->pizza2_inches = ntohs(ptr_bytes1->pizza2_inches);
	ptr_bytes1->pizza2_fractional_inches = ntohs(ptr_bytes1->pizza2_fractional_inches);
	ptr_bytes1->vendor1_name_ln = ntohs(ptr_bytes1->vendor1_name_ln);
	ptr_bytes1->vendor2_name_ln = ntohs(ptr_bytes1->vendor2_name_ln);
	strcpy(vendor1, &ptr_bytes1->vendor1_st);
	strcpy(vendor2, (char *)ptr_bytes1);
	printf("$%i.%i\n",ptr_bytes1->pizza1_dollars, ptr_bytes1->pizza1_cents);
	printf("Diameter of pizza 1: %u.%u\n", ptr_bytes1->pizza1_inches, ptr_bytes1->pizza1_fractional_inches);
	printf("Cost of pizza2: $%u.%u\n",ptr_bytes1->pizza2_dollars, ptr_bytes1->pizza2_cents);
	printf("Diameter of pizza 2: %u.%u\n", ptr_bytes1->pizza2_inches, ptr_bytes1->pizza2_fractional_inches);
	printf("Pizza 1 vendor: \"%s\"\n", vendor1);
	printf("Pizza 2 vendor: \"%s\"\n", vendor2);
	
	close(sockfd);
	freeaddrinfo(res);
	return 0;
}
