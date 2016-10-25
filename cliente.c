#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>

#include "ArgumentAnalizer.h"

typedef int FileDescriptor;
typedef struct sockaddr_in SocketAddress;
typedef struct hostent HostEntry;

#define BUFFER_SIZE 256

#define MODE_NOT_SET 0
#define MODE_UDP 1
#define MODE_TCP 2

void Error(const char *msg)
{
	perror(msg);
	exit(1);
}

void UdpClient(char* hostName, int serverPort);
void TcpClient(char *hostName, int serverPort);

int main(int argc, char *argv[])
{
	int serverPort;
	int aux, mode;
	char *hostName;

	mode= MODE_NOT_SET;
	srand(time(NULL));
	if( SEARCH_FAILED == (aux = FindArgument("-p", argc, argv) ) )
	{
//		serverPort= (rand()%63525) + 2000;
		printf("Server port not specified, plesase rerun with -p [portNumber] to inform the port.\n");
		return 0;
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
		printf("Transmission mode not specified, plesase rerun with -udp or -tcp to inform the transmission mode.\n");
		return 0;
	}
	if( SEARCH_FAILED == (aux = FindArgument("-h", argc, argv) ) )
	{
		printf("Host name not specified, plesase rerun with -h [hostname] to inform the host name.\n");
	}
	else
	{
		hostName= argv[aux+1];
		printf("Host name: %s\n", hostName);
	}

	if(MODE_UDP == mode)
	{
		UdpClient(hostName, serverPort);
	}
	else if(MODE_TCP == mode)
	{
		TcpClient(hostName, serverPort);
	}
	else
	{
		Error("[ERROR] Mode not known\n");
	}
	return 0;
}


void TcpClient(char *hostName, int serverPort)
{
	FileDescriptor clientSocketFD;
	SocketAddress serverAddress;
	int bytesReadOrWritten;
	HostEntry *server;

	char buffer[BUFFER_SIZE];
	
	clientSocketFD= socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocketFD < 0)
	{
		Error("[ERROR] Fail opening socket\n");
	}
	server= gethostbyname(hostName);
	if (NULL == server)
	{
		fprintf(stderr, "[ERROR] Host not found\n");
		exit(1);
	}
	memset((char *) &serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family= AF_INET;
	memcpy(
		(char*)server->h_addr_list[0],
		(char*) &(serverAddress.sin_addr.s_addr),
		server->h_length
	);
	serverAddress.sin_port= htons(serverPort);
	if(connect(
		clientSocketFD,
		(struct sockaddr *) &serverAddress,
		sizeof(serverAddress) 
	) < 0)
	{
		Error("[ERROR] Error connecting");
	}
	printf("Tell  me what you wanna send: ");
	memset(buffer, 0, BUFFER_SIZE);
	fgets(buffer, BUFFER_SIZE-1, stdin);
	bytesReadOrWritten= write(clientSocketFD, buffer, strlen(buffer));
	if(bytesReadOrWritten < 0)
	{
		Error("[ERROR] Error writing to socket\n");
	}
	memset(buffer, 0, BUFFER_SIZE);
	bytesReadOrWritten= read(clientSocketFD, buffer, BUFFER_SIZE-1);
	if (bytesReadOrWritten < 0)
	{
		Error("[ERROR] Error reading from socket\n");
	}
	printf("Message recieved: %s\n", buffer);
	close(clientSocketFD);
}

void UdpClient(char* hostName, int serverPort)
{
	FileDescriptor clientSocketFD;
	int bytesReadOrWritten;
	unsigned int length;
	SocketAddress server, origin;
	HostEntry *host;
	char buffer[BUFFER_SIZE];
	
	clientSocketFD= socket(AF_INET, SOCK_DGRAM, 0);
	if (clientSocketFD < 0)
	{
		Error("[ERROR] Fail opening socket\n");
	}
	host= gethostbyname(hostName);
	if (NULL == host)
	{
		fprintf(stderr, "[ERROR] Host not found\n");
		exit(1);
	}
	server.sin_family= AF_INET;
	server.sin_port= htons(serverPort);
	length= sizeof(SocketAddress);
	printf("Tell  me what you wanna send: ");
	memset(buffer, 0, BUFFER_SIZE);
	fgets(buffer, BUFFER_SIZE-1, stdin);
	bytesReadOrWritten= sendto(clientSocketFD, buffer, strlen(buffer), 0, (struct sockaddr*) &server, length);
	if(bytesReadOrWritten < 0)
	{
		Error("[ERROR] Error writing to socket\n");
	}
//	memset(buffer, 0, BUFFER_SIZE);
	bytesReadOrWritten= recvfrom(clientSocketFD, buffer, BUFFER_SIZE, 0, (struct sockaddr*) &origin, &length);
	if (bytesReadOrWritten < 0)
	{
		Error("[ERROR] Error reading from socket\n");
	}
	printf("Response recieved from server: %s\n", buffer);
	close(clientSocketFD);
}

