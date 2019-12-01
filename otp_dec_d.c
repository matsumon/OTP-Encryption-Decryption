#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg) { perror(msg); exit(1); } // Error function used for reporting issues

int forkCount = 0;

int main(int argc, char *argv[])
{
	int listenSocketFD, establishedConnectionFD, portNumber, charsRead;
	socklen_t sizeOfClientInfo;
	char buffer[160000];
	struct sockaddr_in serverAddress, clientAddress;

	if (argc < 2) { fprintf(stderr,"USAGE: %s port\n", argv[0]); exit(1); } // Check usage & args

	// Set up the address struct for this process (the server)
	memset((char *)&serverAddress, '\0', sizeof(serverAddress)); // Clear out the address struct
	portNumber = atoi(argv[1]); // Get the port number, convert to an integer from a string
	serverAddress.sin_family = AF_INET; // Create a network-capable socket
	serverAddress.sin_port = htons(portNumber); // Store the port number
	serverAddress.sin_addr.s_addr = INADDR_ANY; // Any address is allowed for connection to this process

	// Set up the socket
	listenSocketFD = socket(AF_INET, SOCK_STREAM, 0); // Create the socket
	if (listenSocketFD < 0) error("ERROR opening socket");

	// Enable the socket to begin listening
	if (bind(listenSocketFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) // Connect socket to port
		error("ERROR on binding");
	listen(listenSocketFD, 5); // Flip the socket on - it can now receive up to 5 connections

	// Accept a connection, blocking if one is not available until one connects
	sizeOfClientInfo = sizeof(clientAddress); // Get the size of the address for the client that will connect
	while(1)
	{
		establishedConnectionFD = accept(listenSocketFD, (struct sockaddr *)&clientAddress, &sizeOfClientInfo); // Accept
		if(forkCount > 10)
		{
			printf("Too many forks\n");
			exit(1);
		}
		forkCount++;
		int pid = fork();

		if(pid == 0)
		{
			if (establishedConnectionFD < 0) error("ERROR on accept");

			// Get the message from the client and display it
			memset(buffer, '\0', 160000);
			char message [160000];
			memset(message, '\0', 160000);
			char decrypted[80000];
			while(1)
			{
				charsRead = recv(establishedConnectionFD, message,160000 , 0); // Read the client's message from the socket
				strcpy(buffer+strlen(buffer),message);
				if(buffer[strlen(buffer)-1]=='*')
				{
					break;
				}
				if (charsRead < 0) error("ERROR reading from socket");
				//printf("SERVER: I received this from the client: \"%s\"\n", buffer);
			}
			if(buffer[0] != '2')
			{
				fprintf(stderr,"Only Accepts Connections from otp_dec\n");
				close(establishedConnectionFD); // Close the existing socket which is connected to the client
				return(1);
			}
			char text[strlen(buffer)];
			memset(text,'\0',strlen(buffer));
			strcpy(text,buffer+1);
			int half = (strlen(buffer)-1)/2;
			char key[strlen(buffer)];
			memset(key,'\0',strlen(buffer));
			strcpy(key,text+half);
			text[half-1]='\0';
			//printf("text %s",text);
			key[strlen(key)-1]='\0';
			//	printf("%d %d\n",strlen(text),strlen(key));
			int end = strlen(key);
			int i;
			memset(decrypted,'\0',80000);
			char a[1];
			for(i = 0; i <= end; i++)
			{
				if(text[i] == ' ')
				{
					text[i] = '@';
				}
				if(i == end)
				{
					decrypted[i]='\0';
					break;
				}
				int d = text[i] - key[i];
				while(d < 0)
				{
					d= d + 27;
				}
				while(d >= 27)
				{
					d= d - 27;
				}
				d = d + 64;
				//	printf("d %d\n",d);
				sprintf(a,"%c",d);
				if(a[0]=='@')
				{
					a[0] = ' ';
				}
				decrypted[i]=a[0];
			}
			decrypted[strlen(decrypted)]='*';
			// Send a Success message back to the client
			printf("decrypted %s",decrypted);
			charsRead = send(establishedConnectionFD, decrypted,80001, 0); // Send success back
			if (charsRead < 0) error("ERROR writing to socket");
			close(establishedConnectionFD); // Close the existing socket which is connected to the client
		}
		close(establishedConnectionFD); // Close the existing socket which is connected to the client
	}
	//	close(listenSocketFD); // Close the listening socket
	//	return 0; 
}
