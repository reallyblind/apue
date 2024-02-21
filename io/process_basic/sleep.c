#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
int main(int argc ,char ** argv)
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
		//这样ps axf时子进程就是httpd 100 了
		execl("/usr/bin/sleep","httpd","100",NULL);
		perror("execl()");
		exit(1);
	}
	sleep(1000);

	wait(NULL);

	puts("End!");
	exit(0);

}

