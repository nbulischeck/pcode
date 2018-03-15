#include <stdio.h> /* i/o library */
#include <stdlib.h> /* exit call */
#include <string.h> /* string functions */
#include <netdb.h> /* net functions */
#include <unistd.h> /* closing fds */
#include <arpa/inet.h> /* inet ntop */

#define BUFFSIZE 1024

/*
	Function: createClient
	Description: Sets up a client connection with the specified
		hostname and port.
	Arguments:
		* hostname string
		* port string
	Return Value: A socket file descriptor
*/

int createClient(char *hostname, char *port){
    int sfd, status;
	char s[INET_ADDRSTRLEN];
    struct addrinfo hints, *servinfo, *res;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	status = getaddrinfo(hostname, port, &hints, &servinfo);
	if (status != 0){
		return -1;
	}

    for (res = servinfo; res != NULL; res = res->ai_next) {
		sfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if (sfd == -1){
			continue;
		}
		if (connect(sfd, res->ai_addr, res->ai_addrlen) == -1){
			close(sfd);
			continue;
        }
        break;
    }

    if (res == NULL) {
        fprintf(stderr, "Failed to connect to %s:%s.\n", hostname, port);
        return -1;
    }

    inet_ntop(res->ai_family, res->ai_addr, s, sizeof s);

    freeaddrinfo(servinfo);
	return sfd;
}

int main(int argc, char **argv){
	int sfd;
	char buffer[BUFFSIZE];
	ssize_t bytes_read = 0;

	sfd = createClient(argv[1], argv[2]);
	if (sfd == -1){
		exit(-1);
	}

	while((bytes_read = read(0, buffer, BUFFSIZE))){
    	send(sfd, buffer, bytes_read, 0);
		if (memcmp("exit", buffer, 4) == 0)
			break;
		bytes_read = recv(sfd, buffer, BUFFSIZE, 0);
		buffer[bytes_read] = 0;
		printf("%s", buffer);
		memset(buffer, 0, BUFFSIZE);
	}

    close(sfd);
    return 0;
}
