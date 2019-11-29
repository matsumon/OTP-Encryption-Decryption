#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

void error(const char *msg) { perror(msg); exit(0); } // Error function used for reporting issues

int main(int argc, char *argv[])
{
	int socketFD, portNumber, charsWritten, charsRead;
	struct sockaddr_in serverAddress;
	struct hostent* serverHostInfo;
    
	if (argc < 4) { fprintf(stderr,"USAGE: %s hostname port\n", argv[0]); exit(0); } // Check usage & args
	int j;
	FILE * text = fopen(argv[1],"r");
	FILE * key = fopen(argv[2],"r");
	if(text == NULL || key == NULL)
	{
		fprintf(stderr,"Text or Key file could not be opened\n");
		exit(1);
	}
	char * givenText = NULL;
	char * givenKey = NULL;
	size_t size=80000;
	getline(&givenText,&size,text);
	getline(&givenKey,&size,key);
	fclose(text);
	fclose(key);
	if(strlen(givenText) >= strlen(givenKey))
	{
		fprintf(stderr,"Key Size is Smaller than Text size\n");
		exit(1);
	}
	givenKey[strlen(givenText)]='\0';
	int q;
	for(q=0; q<strlen(givenKey)-1;q++)
	{
		if(givenText[q]< 'A' && givenText[q] != ' ')
		{
			fprintf(stderr,"Bad Input In Text %c \n",givenText[q]);
			exit(1);
		}
		if(givenText[q]> 'Z')
		{
			fprintf(stderr,"Bad Input In Text %c \n",givenText[q]);
			exit(1);
		}
		if(givenKey[q]< 'A' && givenKey[q] != ' ')
		{
			fprintf(stderr,"Bad Input In Key %c \n",givenKey[q]);
			exit(1);
		}
		if(givenKey[q]> 'Z')
		{
			fprintf(stderr,"Bad Input In Key %c \n",givenKey[q]);
			exit(1);
		}


	}	
	// Set up the server address struct
	memset((char*)&serverAddress, '\0', sizeof(serverAddress)); // Clear out the address struct
	portNumber = atoi(argv[3]); // Get the port number, convert to an integer from a string
	serverAddress.sin_family = AF_INET; // Create a network-capable socket
	serverAddress.sin_port = htons(portNumber); // Store the port number
	serverHostInfo = gethostbyname("localhost"); // Convert the machine name into a special form of address
	if (serverHostInfo == NULL) { fprintf(stderr, "CLIENT: ERROR, no such host\n"); exit(0); }
	memcpy((char*)&serverAddress.sin_addr.s_addr, (char*)serverHostInfo->h_addr, serverHostInfo->h_length); // Copy in the address

	// Set up the socket
	socketFD = socket(AF_INET, SOCK_STREAM, 0); // Create the socket
	if (socketFD < 0) error("CLIENT: ERROR opening socket");

	// Connect to server
	if (connect(socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) // Connect socket to address
		error("CLIENT: ERROR connecting");
	char buffer[80000];
	memset(buffer,'\0',80000);
	buffer[0] ='2';
	strcpy(buffer+1,givenText);
	strcpy(buffer+1+strlen(givenText),givenKey);
	//printf("buffer: %s\n " , buffer);
	// Send message to server
	charsWritten = send(socketFD, buffer, strlen(buffer), 0); // Write to the server
	if (charsWritten < 0) error("CLIENT: ERROR writing to socket");
	if (charsWritten < strlen(buffer)) printf("CLIENT: WARNING: Not all data written to socket!\n");

	// Get return message from server
	memset(buffer, '\0', sizeof(buffer)); // Clear out the buffer again for reuse
	char completeMessage[80000];
	memset(completeMessage,'\0',80000);
	charsRead = recv(socketFD, buffer,strlen(givenKey)-1 , 0); // Read data from the socket, leaving \0 at end
	strcat(completeMessage,buffer);
	if (charsRead < 0) error("CLIENT: ERROR reading from socket");
	printf("%s\n", completeMessage);
	strcat(completeMessage,buffer);
	close(socketFD); // Close the socket
	return 0;
}
