#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#include "ArgumentAnalizer.h"

typedef FileDescriptor int;
typedef struct sockaddr_in SocketAddress;
#define BUFFER_SIZE 256

#define MODE_NOT_SET 0
#define MODE_UDP 1
#define MODE_TCP 2

struct serverConnetion
{
	FileDescriptor connectionFD;
	SocketAddress clientAddress;
};
typedef struct serverConnetion ServerConnection;

void Error(const char *msg)
{
	perror(msg);
	exit(1);
}

void UdpServer(int serverPort);
void TcpServer(int serverPort);
void* ServerConnection(void*);

int main(int argc, char *argv[])
{
	int serverPort;
	int aux, mode;

	mode= MODE_NOT_SET;
	srand(time(NULL));
	if( SEARCH_FAILED == (aux = FindArgument("-p", argc, argv) ) )
	{
		serverPort= (rand()%63525) + 2000;
		printf("Server port not specified, automatically the port %d has been chosen\n", serverPort);
	}
	else
	{
		serverPort= atoi(argv[aux+1]);
		printf("Server port: %d\n", serverPort);
	}
	if( SEARCH_FAILED != (aux = FindArgument("-tcp", argc, argv) ) )
	{
		mode = MODE_TCP;
	}
	else if( SEARCH_FAILED != (aux = FindArgument("-udp", argc, argv) ) )
	{
		mode = MODE_UDP;
	}
	else
	{
		mode = rand()%2+1;
		printf("Mode not specified, automatically %s has been chosen\n", (MODE_UDP==mode)?"UDP" : "TCP");
	}

	if(MODE_UDP == mode)
	{
		UdpServer(serverPort);
	}
	else if(MODE_TCP == mode)
	{
		TcpServer(serverPort);
	}
	else
	{
		Error("[ERROR] Mode not known\n");
	}
	return 0;
}

void TcpServer(int serverPort)
{
	FileDescriptor socketFD, socketConnectionFD;
	int clientAddressLenght, bytesReadOrWritten;
	socklen_t clientSocketLenght;
	char buffer[BUFFER_SIZE];
	SocketAddress serverAddress, clientAddress;
	
	socketFD= socket(AF_INET, SOCK_STREAM, 0);
	if (socketFD < 0)
	{
		Error("[ERROR] Fail opening socket\n");
	}
	bzero((char *) &serverAddress, sizeof(serverAddress));
	serverAddress.sin_family= AF_INET;
	serverAddress.sin_addr.s_addr= INADDR_ANY;
	serverAddress.sin_port = htons(serverPort);//htons = host to network endian
	if(0 > ( bind(socketFD, (struct sockaddr *) &serverAddress, sizeof(serverAddress) ) ) )
	{
		Error("[ERROR] Binding failed\n");
	}
	listen(socketConnectionFD,5);
	clilen = sizeof(cli_addr);
	socketConnectionFD = accept(socketFD, (struct sockaddr *) &clientAddress, &clientSocketLenght);
	ServerConnection *sc= malloc(sizeof(ServerConnection));
	if(NULL == sc)
	{
		Error("[ERROR] Alocation memory error\n");
	}
	sc.connectionFD= socketConnectionFD;
	sc.clientAddress= clientAddress;
	pthread_create();
	if(socketConnectionFD < 0)
	{
		error("[ERROR] Error on accept\n");
	}
	bzero(buffer,BUFFER_SIZE);
	bytesReadOrWritten= read(socketConnectionFD, buffer, BUFFER_SIZE-1);
	if (n < 0)
	{
		Error("[ERROR] Error reading from socket\n");
	}
	printf("Recieved message: %s\n",buffer);
}

void* ServerConnection(void* arg)
{
	
}
