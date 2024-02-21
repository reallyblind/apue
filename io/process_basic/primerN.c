#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdio.h>
#include<stdlib.h>

#define LEFT	30000000
#define RIGHT	30000200
#define N	3
int main()
{
	int i ;
	int mark = 1;
	pid_t pid;
	int n;
	for(n = 0 ; n < N ; n++)
	{
		pid = fork();
		if(pid < 0)
		{
			//出错后把fork出去的wait回来
			perror("fork()");
			exit(1);
		}
		if(pid == 0)
		{

			for( i = LEFT + n ; i < RIGHT ; i+=N)
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
					printf("[%d]%d is a primer\n",n,i);
			}
			exit(0);
		}
	}	
	int st;
	for(n = 0 ; n < N ; n++)
//		wait(&st);
		wait(NULL);


	exit(0);
}

