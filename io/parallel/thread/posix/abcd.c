#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<unistd.h>
#include<sys/types.h>

#define THRNUM		4

static pthread_mutex_t mut[THRNUM];
static int next(int n)
{
	return n+1==THRNUM? 0 :n+1;
}

static void * thr_func(void * p)
{
	int n = (int)p;
	int c = 'a' + n ;

	while(1)
	{
		pthread_mutex_lock(mut+n);//main中直接锁上了,此时a线程想要这个锁要不到,所以等待
		write(1,&c,1);
		pthread_mutex_unlock(mut+next(n));//锁自己,等下一个人
	}
	pthread_exit(NULL);
}


int main()
{
	pthread_t tid[THRNUM];
	int i ,err;
	for(i = 0 ; i < THRNUM ; i++)
	{
		pthread_mutex_init(mut+i,NULL);
		pthread_mutex_lock(mut+i);

		err = pthread_create(tid+i,NULL,thr_func,(void * )i);
		if(err)
		{
			fprintf(stderr,"pthread_create():%s\n",strerror(err));
			exit(1);
		}
	}
	pthread_mutex_unlock(mut+0);

	alarm(5);

	for( i = 0 ; i < THRNUM ; i++)
		pthread_join(tid[i],NULL);


	exit(0);
}
