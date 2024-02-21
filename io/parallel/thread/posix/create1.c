#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

static void *func(void *p)
{
	puts("Thread is working");
	return NULL;
}

int mian()
{
	puts("BEGIN");

	pthread_t tid;
	pthread_create(&tid,NULL,func,NULL);



	puts("END");
	exit(0);
}


