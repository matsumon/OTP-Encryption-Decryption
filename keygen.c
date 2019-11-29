#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
int main(int argc, char * argv[])
{
	srand(time(NULL));
	if(argc != 2)
	{
		fprintf(stderr,"Incorrect Number of Arguments\n");
		exit(1);
	}
	if(atoi(argv[1])<=0)
	{
		fprintf(stderr,"Invalid Number of Characters For Key\n");
		exit(1);
	}
	int i;
	char key[80000];
	memset(key,'\0',80000);
	int end = atoi(argv[1]);
	for(i = 0; i < end; i++)
	{
		int g = rand()%27 + 64;
		if(g=='@')
		{
			g = ' ';
		}
		key[i]=g;
	}
	printf("%s\n",key);
}

