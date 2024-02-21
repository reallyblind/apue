#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#define LEFT	30000000
#define RIGHT	30000200
int main()
{
	int mark = 1;
	pid_t pid;
	for(int i = LEFT ; i < RIGHT ; i++)
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
			//假如此处sleep存在,下一个sleep不存在,子进程跑完不收尸,就变成了僵尸进程,父进程跑完结束,自动交给init接管,由init收尸
	//		sleep(1000);
	//		init在子进程退出收尸,此处是exit
			exit(0);
		}
	}
//父进程slepp 1000,子进程算完直接走
//假如此处sleep存在,上一个sleep不存在,那么子进程结束变成僵尸模块但仍然是由父进程接管的
	sleep(1000);
	
	exit(0);
}

