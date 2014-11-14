/* A simple server in the internet domain using TCP.
myServer.c
D. Thiebaut
Adapted from http://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html
The port number used in 51717.
This code is compiled and run on the Raspberry as follows:

    g++ -o myServer myServer.c 
    ./myServer

The server waits for a connection request from a client.
The server assumes the client will send positive integers, which it sends back multiplied by 2.
If the server receives -1 it closes the socket with the client.
If the server receives -2, it exits.
 */

#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>


void error( char *msg ) {
	perror(  msg );
	exit(1);
}

int func( int a ) {
	return 2 * a;
}

void sendData( int sockfd, int x ) {
	int n;

	char buffer[32];
	sprintf( buffer, "%d\n", x );
	if ( (n = write( sockfd, buffer, strlen(buffer) ) ) < 0 )
	{
		perror("ERROR writing to socket");
	}
	buffer[n] = '\0';
}

int getData( int sockfd, char * data ) {
	char buffer[32];
	int n;
	int i=0;
	int j=0;
	bzero(data, sizeof(data));
	if ( (n = recv(sockfd,buffer,31) ) < 0 )
	{
		perror("ERROR reading from socket");
	}
	for (i=0; i < strlen(buffer); i++)
	{
		if (isalpha(buffer[i]))
		{
			data[j] = buffer[i];
			j++;
		}
	}
	buffer[n] = '\0';
	printf("buffer is %s lenght of bufer is %d data readin is %d\n", data, strlen(data),n );
	return atoi( buffer );
}

int main(int argc, char *argv[]) {
	int sockfd, newsockfd, portno = 51719, client;
	char buffer[1024];
	struct sockaddr_in serverAddress, clientAddress;
	int n;
	int data;

	printf( "using port #%d\n", portno );

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		perror("ERROR opening socket");
	}
	bzero((char *) &serverAddress, sizeof(serverAddress));

	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons( portno );
	if (bind(sockfd, (struct sockaddr *) &serverAddress,sizeof(serverAddress)) < 0)
	{
		perror("ERROR on binding");
	}
	listen(sockfd,5);
	client = sizeof(clientAddress);

	//--- infinite wait on a connection ---
	while ( 1 ) {
		printf( "waiting for new client...\n" );
		if ( ( newsockfd = accept( sockfd, (struct sockaddr *) &clientAddress, (socklen_t*) &client) ) < 0 )
		{
			perror("ERROR on accept");
		}
		printf( "opened new communication with client\n" );
		while ( 1 ) {
			//---- wait for a number from client ---
			getData( newsockfd, buffer );
			printf( "got %d\n", data );
			if ( strcmp(buffer,"quit") == 0)
			{
				printf( "got quit\n");
				break;
			}
			
			data = func( data );

			//--- send new data back ---
			printf( "sending back %d\n", data );
			sendData( newsockfd, data );
		}
		close( newsockfd );

		//--- if -2 sent by client, we can quit ---
		if ( strcmp(buffer,"quit") == 0 )
			break;
	}
	return 0;
}

void processCommands(char * commands)
{

}
