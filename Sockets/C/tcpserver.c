#include <stdio.h> /* i/o library */
#include <stdlib.h> /* memory allocation */
#include <string.h> /* string functions */
#include <netdb.h> /* net functions */

#define BUFFSIZE 1024

/*
	Function: createServer
	Description: Sets up a server on localhost
		with the specified port. This is one step away
		from accepting clients.
	Arguments:
		* port string
	Return Value: A socket file descriptor
*/

int createServer(char *port){
	int status, sfd;
	struct addrinfo hints, *res;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	status = getaddrinfo(NULL, port, &hints, &res);
	if (status != 0){
		return -1;
	}

	sfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (sfd == -1){
		return -1;
	}

	status = bind(sfd, res->ai_addr, res->ai_addrlen);
	if (status == -1){
		return -1;
	}

	freeaddrinfo(res);
	return sfd;
}

int main(int argc, char **argv){
	int s, sfd, cfd;
	ssize_t read;
	char buffer[BUFFSIZE], hold[BUFFSIZE+1];
	struct sockaddr_storage client;
	socklen_t sin_size = sizeof client;

	if (argc != 2){
		printf("You must have a port number.\n");
		return -1;
	}

	sfd = createServer(argv[1]);
	if (sfd == -1){
		return -1;
	}

	s = listen(sfd, SOMAXCONN);
	if (s == -1){
		return -1;
	}

	cfd = accept(sfd, (struct sockaddr *) &client, &sin_size);
	if (cfd == -1) {
		perror("accept");
	}

	while((read = recv(cfd, buffer, BUFFSIZE, 0))){
		if (memcmp("exit", buffer, 4) == 0)
			break;
		strncpy(hold, buffer, read), hold[read] = 0;
		printf("Received: %s", hold);
		send(cfd, hold, read+1, 0);
	}

	return 0;
}
