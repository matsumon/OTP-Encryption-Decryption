#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
void generateKey(char *, char *);
char * encrypt(char*,char*);
char * decrypt(char *, char*);
void sentenceSpace(char*);
void check(char origin [100],char * destination)
{
	if(strcmp(origin,destination)==0)
		exit(0);
	else 
		printf("bad");
		exit (1);
}
int main()
{
	srand(time(NULL));
	char sentenceToEncrypt[100]= "ABCDEFGHIJKL MNOPQRSTUVWXYZ";
	sentenceSpace(sentenceToEncrypt);
	printf("sentence %s\n",sentenceToEncrypt);
	char key [100];
	memset(key,'\0',100);
	generateKey(sentenceToEncrypt,key);	
	printf("key %s\n",key);
	char * encryptedSentence = encrypt(sentenceToEncrypt,key);
	printf("encrypted %s\n",encryptedSentence);
	char * decryptedSentence;
	decryptedSentence=decrypt(encryptedSentence,key);
	printf("sentence %s\n",sentenceToEncrypt);
	printf("dencrypt %s\n",decryptedSentence);
	sentenceSpace(decryptedSentence);
	check(sentenceToEncrypt,decryptedSentence);
}
void sentenceSpace(char sentence [100])
{
	int i;
	for(i = 0; i < strlen(sentence); i++)
	{
		if(sentence[i] ==' ')
		{
			sentence[i] = '@';
		}
	}
}
void generateKey(char sentence[100], char * key)
{
	int i;
	int end = strlen(sentence);
	for(i = 0; i < end; i++)
	{
		int g = rand()%27 + 64;
		key[i]=g;
	}
}
char * encrypt(char sentence[100],char*key)
{
	char * encrypt;
	encrypt = (char*) malloc(sizeof(char)*strlen(sentence));	
	char a [1];
	int d;
	for(d = 0; d < strlen(sentence); d++)
	{
		int i = (sentence[d]-64) + (key[d]-64);
		i = i % 27;
		i = i + 64;
		sprintf(a,"%c",i);
		encrypt[d]=a[0];
	}
	return(encrypt);
}
char * decrypt(char sentence [100], char * key)
{
	int end = strlen(sentence);
	int i;
	char * decrypted;
	 decrypted = (char *) malloc(sizeof(char)*strlen(sentence));
	memset(decrypted,'\0',strlen(sentence));
	char a[1];
	for(i = 0; i <= end; i++)
	{
		if(i == end)
		{
			decrypted[i]='\0';
			break;
		}
		int d = sentence[i] - key[i];
		if(d < 0)
		{
			d= d + 27;
		}
		if(d > 27)
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
	return(decrypted);
}
