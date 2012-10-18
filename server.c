// Name & E-mail: Celena Tan 			tan.celena@gmail.com
// Name & E-mail: Aseda Aboagye			aseda.aboagye@gmail.com
// File Name: server.c
// Course: COMP 177 Computer Networking
// Project: Pizza Consumers Union

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
#include <math.h>
#include <signal.h>
#include "server.h"

#define QUEUE 10
#define MAX_BUF_SIZE 1024
#define MAX_NAME_SIZE 255

int sockfd=0, sockfd_client=0;

void graceful(int signo){
	if(signo == SIGINT){
		close(sockfd);
		close(sockfd_client);
		exit(0);
	}
}

int main(int argc, char **argv){
	if(argc < 2){
		printf("Usage:	%s <server port>\n", argv[0]);
		return 1;
	}
	signal(SIGINT, graceful);
	while(1){
		struct addrinfo hints, *res;
		memset(&hints, 0, sizeof hints);
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_PASSIVE;
		int status = getaddrinfo(NULL, argv[1], &hints, &res);
		if(status !=0){
			fprintf(stderr, "getaddrinfo ERROR: %s\n",gai_strerror(status));
			return 1;
		}
		sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if(sockfd<0){
			perror("server: socket");
			return 1;
		}/* rerun the server immediately after we kill it otherwise we have to wait about 20 secs.
		 * Eliminates "ERROR on binding: Address already in use" error. */
	    int optval = 1;
		setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval , sizeof(int));
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
		struct sockaddr_storage their_addr;
		socklen_t addr_size = sizeof their_addr;
		sockfd_client = accept(sockfd, (struct sockaddr *) &their_addr, &addr_size);
		if(sockfd_client == -1){
			perror("server: accept");
			return 1;
		}
		/*	At this point, the socket is set up for the server and has accepted a
		 *	client's connect(). */
		char buffer[MAX_BUF_SIZE];
		memset(buffer, 0, MAX_BUF_SIZE);
		/* read in transmission from client. */
		/* Now I need to separate the data into the respective fields */
		char vendor1[MAX_NAME_SIZE];
		char vendor2[MAX_NAME_SIZE];
		int total_size1 = sizeof(struct parlor) -1 + MAX_NAME_SIZE;
		char* buffer_ptr1 = malloc(total_size1);
		struct parlor* ptr_bytes1;
		ptr_bytes1 = (struct parlor*) buffer_ptr1;
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
		strncpy(vendor1, (char*) (ptr_bytes1 + 1), ptr_bytes1->vendor1_name_ln);
		strcpy(vendor2, (char*) (ptr_bytes1 + 1) +(ptr_bytes1->vendor1_name_ln + 1));
		
		struct calc winner;
		double cpsi1, cpsi2;
		double p1area = (double)ptr_bytes1->pizza1_inches;
		double frac = (double)ptr_bytes1->pizza1_fractional_inches;
		frac = frac / 100;
		p1area += frac; p1area /= 2; p1area *= p1area; p1area *= 3.14159;

		cpsi1 = (double)ptr_bytes1->pizza1_dollars;
		double cents = (double)ptr_bytes1->pizza1_cents;
		winner.p1dol = ptr_bytes1->pizza1_dollars;
		cents /= 100;
		double temp1 = (double)winner.p1dol;
		temp1 += cents;											/* Adding decimal cents*/
		cpsi1 += cents; cpsi1 /= p1area;
		winner.p1cen = (uint16_t)(100 * modf(cpsi1, &temp1));	/* Splitting double value into 2 doubles */
		winner.p1dol = (uint16_t) temp1;						/* Storing back into the struct. */

		double p2area = (double)ptr_bytes1->pizza2_inches;
		frac = (double)ptr_bytes1->pizza2_fractional_inches;
		frac = frac / 100;										/* convert to decimal inches */
		p2area += frac; p2area /= 2; p2area *= p2area; p2area *= 3.14159;

		cpsi2 = (double)ptr_bytes1->pizza2_dollars;
		cents = (double)ptr_bytes1->pizza2_cents;
		cents /= 100;
		winner.p2dol = ptr_bytes1->pizza2_dollars;
		winner.p2cen = ptr_bytes1->pizza2_cents;
		cpsi2 += cents; cpsi2 /= p2area;
		double temp2 = (double)winner.p2dol;
		temp2 += cents;
		winner.p2cen = (uint16_t)(100 * modf(cpsi2, &temp2));
		winner.p2dol = (uint16_t) temp2;

		char *buffer_ptr2 = malloc(sizeof(struct calc) + MAX_NAME_SIZE);
		memset(buffer_ptr2, 0, (sizeof(struct calc) + MAX_NAME_SIZE));
		struct calc *send_ptr = (struct calc*)buffer_ptr2;
		send_ptr->p1dol = htons(winner.p1dol);
		send_ptr->p1cen = htons(winner.p1cen);
		send_ptr->p2dol = htons(winner.p2dol);
		send_ptr->p2cen = htons(winner.p2cen);

		int bytes_sent;
		if(cpsi1 > cpsi2){
			winner.win_len = (uint16_t)strlen(vendor2);
			send_ptr->win_len = htons(winner.win_len);
			strncpy((char*)(send_ptr + 1), vendor2, strlen(vendor2));
			bytes_sent = send(sockfd_client, (char*)send_ptr, sizeof(struct calc) + strlen(vendor2) + 1, 0);
			if(bytes_sent == -1){
				perror("server: send");
				return 1;
			}
		}else{
			winner.win_len = (uint16_t)strlen(vendor1);
			send_ptr->win_len = htons(winner.win_len);
			strncpy((char*)(send_ptr + 1), vendor1, strlen(vendor1));
			bytes_sent = send(sockfd_client, (char*)send_ptr, sizeof(struct calc) + strlen(vendor1) + 1, 0);
			if(bytes_sent == -1){
				perror("server: send");
				return 1;
			}
		}
		close(sockfd);
		close(sockfd_client);
		free(buffer_ptr1);
		free(buffer_ptr2);
		freeaddrinfo(res);
	}
	return 0;
}
