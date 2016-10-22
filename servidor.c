#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#include "ArgumentAnalizer.h"

typedef FileDescriptor int;
typedef struct sockaddr_in SocketAddress;
#define BUFFER_SIZE 256

#define MODE_NOT_SET 0
#define MODE_UDP 1
#define MODE_TCP 2

void Error(const char *msg)
{
	perror(msg);
	exit(1);
}

void UdpServer(int serverPort);
void TcpServer(int serverPort);

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
	socklen_t clilen;
	char buffer[BUFFER_SIZE];
	SocketAddress serverAddress, clienteAddress;
}

