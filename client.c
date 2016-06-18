/****************************************************************************************
** Author: James Le
** Date: 4/28/2016
** Main Reference: http://www.linuxhowtos.org/C_C++/socket.htm
** Some Reference: http://beej.us/guide/bgnet/output/html/singlepage/bgnet.html
*****************************************************************************************/


#include <stdio.h>  // contains declarations used in most input and output
#include <sys/types.h>  // contains number of data types used in system calls
#include <sys/socket.h>  // structures needed for sockets
#include <netinet/in.h>  // structures needed for internet domain address
#include <netdb.h>   // defines the structure hostent
#include <string.h>  // for string inputs
#include <stdlib.h>  // standard library
#include <unistd.h>  // for various constant, type, and function declarations


void startChat(int socket);


int main(int argc, char *argv[])
{

	/* Variables */
	int sockFd;
	int portNum = atoi(argv[2]);

	struct sockaddr_in server;
	struct hostent *hn;  // host computer's info

	
	/* Create socket */
	
	sockFd = socket(AF_INET, SOCK_STREAM, 0);

	if(sockFd < 0)
	{
		perror("Socket failed");
		close(sockFd);
		exit(1);

	}

	else
	{
		printf("Connected to the server\n");
	}


	 
	server.sin_family = AF_INET;


	// argv[1] contains the name of the host on the internet
	// takes server info 
	hn = gethostbyname (argv[1]);

	if(hn == 0)
	{
		perror("gethostbyname failed");
		close(sockFd);
		exit(1);

	}

	// set fields for server again
	// char string, so using memcpy to copy over length bytes
	memcpy(&server.sin_addr, hn->h_addr, hn->h_length); 
	server.sin_port = htons(portNum);

	/* Connect socket */

	if(connect(sockFd, (struct sockaddr *) &server, sizeof(server)) < 0)
	{
		perror("Connection failed");
		close(sockFd);
		exit(1);

	}

	else
	{
		startChat(sockFd);
	}


	close(sockFd);
	return 0;
}




// start chat system between server and client
void startChat(socket)
{
	int rVal;   // number of chars to read or write
	int quit = 0;
	
	char msgReceived[500];     // message to be received by client
	char msgSent[500];		 // message to be sent to server
	char msgsentHandle[510];  // message sent with the handle name
	char handleName[11];   // handle name or username
	char *endLine;



	/* Create handle name */
	bzero(handleName, 11);
	printf("What's your handle name: ");
	fgets(handleName, 10, stdin);

	// replace new line char with \0 created by fgets
	if((endLine = strchr(handleName, '\n')) != 0 )
	{
		*endLine= '\0';
	}


	/* Start chat loop */
	while(quit == 0)
	{
		printf("Write a message: ");
		bzero(msgSent, 500);
		fgets(msgSent, 500, stdin);


		// for placing handle name in the front
		sprintf(msgsentHandle, "%s >> %s", handleName, msgSent);

		if(strstr(msgSent, "\\quit") == 0)
		{
			// write to server
			rVal = write(socket, msgsentHandle, strlen(msgsentHandle));
			
			// messages received from server
			bzero(msgReceived, 500);

			// no extra chars since server name is hardcoded
			rVal = recv(socket, msgReceived, 500, 0);

			// the server has quitted if value is equal to 0
			if(rVal == 0)
			{
				printf("Disconnected from the server.\n");
				quit = 1;

			}

			else
			{
				printf("Chatserv >> %s", msgReceived);
			}

		}

		// if \quit was typed
		else
		{
			quit = 1;
		}
	}

	close(socket);

}

