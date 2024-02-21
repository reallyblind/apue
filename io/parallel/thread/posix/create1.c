#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>

static void *func(void *p)
{
	puts("Thread is working");
	pthread_exit(NULL);
//	return NULL;
}

int main()
{
	puts("BEGIN");
	int err;
	pthread_t tid;
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


