#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
int main()
{
	puts("Begin!");
	pid_t pid;

	fflush(NULL);

	pid = fork();
	if(pid < 0)
	{
		perror("fork()");
		exit(1);
	}

	if(pid == 0)
	{
		execl("/bin/date","date","+%s",NULL);
		perror("execl()");
		exit(1);
	}
	sleep(1000);

	wait(NULL);

	puts("End!");
	exit(0);

}

