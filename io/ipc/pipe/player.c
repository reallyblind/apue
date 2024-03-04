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
		close(pd[1]);	//关闭写端
		
		dup2(pd[0],0);//读端重定向到stdin
		close(pd[0]);
		fd = open("dev/null",O_RDWR);
		dup2(fd,1);
		dup2(fd,2);


		execl("~/ffmpeg6.0_sources/FFmpeg","ffplay","-",NULL);
		perror("execl()");
		exit(1);
		

	}
	//parent write
	
	close(pd[0]); //close read
	//父进程从网上收数据,往管道中写

	close(pd[1]);
	wait(NULL);


	exit(0);
}


