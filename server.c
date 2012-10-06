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

#define QUEUE 10

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
	return 0;
}
