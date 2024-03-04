
//此代码没有收尸为什么线程也消失了,不占tid(LWP)了

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<error.h>
#include<pthread.h>
#include"mysemaphore.h"
#include<string.h>
#define LEFT	30000000
#define RIGHT	30000200
#define THRNUM	(RIGHT-LEFT+1)
#define N	4
static mysem_t * sem;
static void *thr_prime(void *p)
{
	int j;
	int i =(int) p;
	int mark = 1;



	for(j = 2 ; j < i/2 ; j++)
	{
		if(i%j == 0)
		{
			mark = 0;
			break;
		}
	}
	if(mark)
		printf("%d is a primer\n",i);

	sleep(5);

	mysem_add(sem,1);
	pthread_exit(NULL);
}

int main()
{
	int i,j, mark = 1;
	pthread_t tid[THRNUM];
	int err;

	sem = mysem_init(N);
	if(sem == NULL)
	{
		fprintf(stderr,"mysem_init()...\n");
		exit(1);
	}

	for( i = LEFT ; i <= RIGHT ; i++)
	{
		mysem_sub(sem,1);
		err = pthread_create(tid+(i-LEFT),NULL,thr_prime,(void *)i);
		if(err)
		{
			fprintf(stderr,"pthread_create():%s\n",strerror(err));
			exit(1);

			for(j = LEFT;j < i ; j++)
			pthread_join(tid[j - LEFT],NULL);
		}
	}

	for(i = LEFT; i <= RIGHT ; i++)
		pthread_join(tid[i-LEFT],NULL);



	exit(0);
}
/*

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
			exit(0);
		}
	}

	exit(0);
}
*/
