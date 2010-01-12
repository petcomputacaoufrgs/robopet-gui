#ifdef WIN32
#include <winsock.h>
#include <process.h>
#include <io.h>
#include <stdio.h>
#else
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#endif

#include "server.h"
#include <errno.h>

void* wrapper(void* serv)
{
	Server* object = (Server*) serv;
	object->handler();
	return NULL;
}

void Server::handler()
{
  puts("Server::handler started");

	while(recvdata(sock, buffer, sizeOfPacket) > 0)
	{
		pthread_testcancel();
		//* Printfs
		printf("%d\n", *(int*)  buffer);
		printf("%d\n", *(int*)  (buffer+sizeof(int)));
		printf("%d\n", *(int*)  (buffer+2*sizeof(int)));
		int i;
		for(i=0; i<(int)*buffer; i++)
		    printf("%d\n", *(int*)  (buffer+(3+i)*sizeof(int)));
		printf("~endOfPackage~\n");
		//*/
  }

  puts("Server::handler reached EOF on socket");
	if (errno != 0)
		printf("Server::handler error code: %s\n", strerror(errno));
}

void Server::socklisten()
{
    struct sockaddr sockaddr;
    int addrlen = sizeof(sockaddr);

   	if (listen(sock, 5) == -1)
	{
		sockerror("Server::socklisten(listen)\n");
		exit(0);
	}
	memset(&sockaddr, 0, sizeof(sockaddr));
	sock = accept(sock, &sockaddr, (socklen_t*) &addrlen);
	if (sock != -1)
		pthread_create(&thread_id, NULL, wrapper, this);
	else
	{
		sockerror("Server::socklisten(accept)\n");
		exit(0);
	}
}

Server::Server(unsigned int sizeOfPacket, int port)
{
		this->sizeOfPacket = sizeOfPacket;
		buffer = new char[sizeOfPacket];
		sock = sockopen(NULL, port);

    if (sock == -1)
		{
        perror("Server::server.sockopen\n");
        exit(1);
    }

    sockinfo(sock, (char*) info);
    printf("Server open.");
    //printf("Server socket open on host %d.%d.%d.%d, port %d\n",
    //       info[0], info[1], info[2], info[3], info[4] * 256 + info[5]);

    socklisten();
}

Server::~Server()
{
		delete[] buffer;
		pthread_cancel(thread_id);
}
