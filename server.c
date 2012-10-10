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
	char vendor1[MAX_BUF_SIZE];
	char vendor2[MAX_BUF_SIZE];
	int total_size1 = sizeof(struct parlor) - 1 + strlen(vendor1) + 1;
	int total_size2 = sizeof(struct parlor) - 1 + strlen(vendor2) + 1;
	char* buffer_ptr1 = malloc(total_size1);
	char* buffer_ptr2 = malloc(total_size2);
	struct parlor* ptr_bytes1;
	struct parlor* ptr_bytes2;
	ptr_bytes1 = (struct parlor*) buffer_ptr1;
	ptr_bytes2 = (struct parlor*) buffer_ptr2;
	
	/* here there needs to be something along the lines of: */
	recv(sockfd_client, buffer, 2, 0); /* receiving 16 bits = 2 bytes */
	ptr_bytes1->pizza_dollars = (uint16_t) buffer;
	recv(sockfd_client, buffer, 2, 0);
	ptr_bytes1->pizza_cents = (uint16_t) buffer;	
	printf("$%i.%i\n",ptr_bytes1->pizza_dollars, ptr_bytes1->pizza_cents);
	
	close(sockfd);
	freeaddrinfo(res);
	return 0;
}
