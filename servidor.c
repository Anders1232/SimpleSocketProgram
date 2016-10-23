#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#include "ArgumentAnalizer.h"
#include "vector.h"

typedef int FileDescriptor;
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
void* ConnectionEstabilished(void*);

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

static bool GetSetStopFlag(bool isToSet)
{
	static bool flag=false;
	if(isToSet)
	{
		flag=true;
	}
	return flag;
}

void TcpServer(int serverPort)
{
	FileDescriptor serverSocketFD, socketConnectionFD;
	socklen_t clientSocketLenght;
	SocketAddress serverAddress, clientAddress;
	
	Vector *connectionsList= NewVector(sizeof(ServerConnection));
	Vector *threadsList= NewVector(sizeof(pthread_t));
	
	serverSocketFD= socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocketFD < 0)
	{
		Error("[ERROR] Fail opening socket\n");
	}
	memset((char *) &serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family= AF_INET;
	serverAddress.sin_addr.s_addr= INADDR_ANY;
	serverAddress.sin_port = htons(serverPort);//htons = host to network endian
	if(0 > ( bind(serverSocketFD, (struct sockaddr *) &serverAddress, sizeof(serverAddress) ) ) )
	{
		Error("[ERROR] Binding failed\n");
	}
	listen(serverSocketFD,5);
	clientSocketLenght = sizeof(clientAddress);
	while(!GetSetStopFlag(false))
	{
		socketConnectionFD = accept(serverSocketFD, (struct sockaddr *) &clientAddress, &clientSocketLenght);
		if(socketConnectionFD < 0)
		{
			Error("[ERROR] Error on accept\n");
			continue;
		}
		ServerConnection sc;/*= malloc(sizeof(ServerConnection));
		if(NULL == sc)
		{
			Error("[ERROR] Alocation memory error\n");
		}*/
		sc.connectionFD= socketConnectionFD;
		sc.clientAddress= clientAddress;
		pthread_create(
				VectorGetElement(threadsList, VectorAllocateOne(threadsList)), NULL,
				ConnectionEstabilished, VectorGetElement(connectionsList, VectorAppendCopy(connectionsList, &sc) )
		);
	}
	for(int count =0; count < threadsList->numberOfElements; count++)
	{
		pthread_t *tread= VectorGetElement(threadsList, count);
		pthread_join( *tread, NULL);
	}
	close(serverSocketFD);
}

void* ConnectionEstabilished(void* arg)
{
	ServerConnection connection= *((ServerConnection*)arg);
	char buffer[BUFFER_SIZE];
	int bytesReadOrWritten;
	
	memset(buffer, 0,BUFFER_SIZE);
	bytesReadOrWritten= read(connection.connectionFD, buffer, BUFFER_SIZE-1);
	if (bytesReadOrWritten < 0)
	{
		Error("[ERROR] Error reading from socket\n");
	}
	printf("Recieved message: %s\n",buffer);
	char* returnMessage= "Message recieved with sucess!";
	bytesReadOrWritten= write(connection.connectionFD, returnMessage, strlen(returnMessage));
	if(bytesReadOrWritten < 0)
	{
		Error("[ERROR] Error writing to socket\n");
	}
	close(connection.connectionFD);
	return NULL;
}

void UdpServer(int serverPort)
{

}
