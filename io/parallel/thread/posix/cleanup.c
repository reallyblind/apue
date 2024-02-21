#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<pthread.h>

static void cleanup_func(void * p)
{
	puts(p);
}

static void * func(void *p)
{
	puts("Thread is working!");
	pthread_cleanup_push(cleanup_func,"CLEANUP:1");
	pthread_cleanup_push(cleanup_func,"CLEANUP:2");
	pthread_cleanup_push(cleanup_func,"CLEANUP:3");

	puts("push over");

	pthread_cleanup_pop(1);
	pthread_cleanup_pop(1);
	pthread_cleanup_pop(1);

	pthread_exit(NULL);
}

int main()
{
	pthread_t tid;
	puts("BEGIN!");
	int err;
	err = pthread_create(&tid,NULL,func,NULL);

	if(err)
	{
		fprintf(stderr,"pthread_create():%s\n",strerror(err));
		exit(1);
	}

	pthread_join(tid,NULL);		

	puts("END");
	exit(0);
}

