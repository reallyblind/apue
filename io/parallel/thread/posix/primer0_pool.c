#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<error.h>
#include<pthread.h>
#include<string.h>
#include<sched.h>
#define LEFT	30000000
#define RIGHT	30000200
#define THRNUM	4

static int num;
static pthread_mutex_t mut_num = PTHREAD_MUTEX_INITIALIZER;

static void *thr_prime(void *p)
{
	int j;
	int mark = 1;
	while(1){
		pthread_mutex_lock(&mut_num);
		//临界区
		while(num == 0)
		{
			pthread_mutex_unlock(&mut_num);
			sched_yield();
			pthread_mutex_lock(&mut_num);
		}
		if(num == -1)
		{
			pthread_mutex_unlock(&mut_num);
			break;//此处要跳转到临界区外,所以要注意锁的情况
		}
			
		int i = num;
		num = 0;
		//临界区
		pthread_mutex_unlock(&mut_num);

		for(j = 2 ; j < i/2 ; j++)
		{
			if(i%j == 0)
			{
				mark = 0;
				break;
			}
		}
		if(mark)
			printf("[%d]:%d is a primer\n",(int)p,i);
	}
	pthread_exit(NULL);
}

int main()
{
	int i,j, mark = 1;
	pthread_t tid[THRNUM];
	int err;
	for( i = 0; i <= THRNUM; i++)
	{
		err = pthread_create(tid+i,NULL,thr_prime,(void *)i);
		if(err)
		{
			fprintf(stderr,"pthread_create():%s\n",strerror(err));
			exit(1);

			for(j = 0;j < j-i ; j++)
			pthread_join(tid[j],NULL);
		}
	}

	for(i = LEFT ; i <= RIGHT ; i++)
	{
		pthread_mutex_lock(&mut_num);
		while(num != 0)//不等于0,说明任务没被接受,解锁出让调度器,等待num用完下发下一个任务
		{
			pthread_mutex_unlock(&mut_num);
			
			sched_yield();//针对调度器使用的,出让调度器给其他线程,很短的等待,并且不会像sleep那样产生调度颠簸,即可避免由running态变为可中断的睡眠态还能出让调度器

			pthread_mutex_lock(&mut_num);
		}
		num = i;
		pthread_mutex_unlock(&mut_num);
	}
	//确保最后一个num被拿走
	pthread_mutex_lock(&mut_num);
	while(num!=0)
	{
		pthread_mutex_unlock(&mut_num);
		sched_yield();
		pthread_mutex_lock(&mut_num);
	}
	num = - 1;
	pthread_mutex_unlock(&mut_num);
	for(i = 0; i <= THRNUM ; i++)
		pthread_join(tid[i],NULL);

	pthread_mutex_destroy(&mut_num);

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
