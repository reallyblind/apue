#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

#define BUFFSIZE	1024

int main()
{
	int pd[2];
	pid_t pid;
	char buf[BUFFSIZE];
	if(pipe(pd) < 0 )
	{
		perror("pipe()");
		exit(1);
	}
	

	pid = fork();
	if(pid < 0)
	{
		perror("fork()");
		exit(1);
	}

	if(pid == 0)//child read
	{
		int len;
		close(pd[1]);
		len = read(pd[0],buf,BUFFSIZE);	
		write(1,buf,len);
		close(pd[0]);
		exit(0);


	}
		//parent write
	
	close(pd[0]); //close read
	write(pd[1],"HELLO!",6);
	close(pd[1]);
	wait(NULL);
	exit(0);


	exit(0);
}


