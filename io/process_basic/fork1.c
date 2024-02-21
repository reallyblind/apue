#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>


int main()
{
	pid_t pid;
	printf("[%d] Begin!\n",getpid());
	

	//fork前一定要刷新缓冲区,否则子进程的缓冲区也有一句begin
	fflush(stdout);

	pid = fork();
	int i = 0;
	if(pid < 0)
	{
		perror("fork()");
		exit(1);
	}

	if(pid ==0 )
	{
		printf("[%d]:child is working!\n",getpid());
	}
	else
	{
		printf("[%d]:Parent is working!\n",getpid());
	}

	printf("[%d]End!\n",getpid());



	getchar();

	exit(0);
}
