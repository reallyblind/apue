#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdio.h>
#include<stdlib.h>

#define LEFT	30000000
#define RIGHT	30000200
int main()
{
	int i ;
	int mark = 1;
	pid_t pid;
	for( i = LEFT ; i < RIGHT ; i++)
	{
		pid = fork();
		if(pid < 0)
		{
			perror("fork()");
			exit(1);
		}
		if(pid == 0 )
		{
			mark = 1;
			for(int j = 2 ; j < i/2 ; j++)
			{
				if( i % j == 0)
				{
					mark = 0;
					break;
				}
			}
			if(mark)
				printf("%d is a primer\n",i);
			//自进程退出,否则自进程将再次fork子进程
			//第一个 fork 199个
			//第二个 fork 198个
			//所以要加上这句exit让子进程退出,防止再fork进程
	//		init在子进程退出收尸,此处是exit
			exit(0);
		}
	}
	
	int st;
	for(i = LEFT ; i <= RIGHT ; i++)
//		wait(&st);
		wait(NULL);


	exit(0);
}

