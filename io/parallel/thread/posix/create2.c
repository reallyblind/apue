#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<string.h>
#include<unistd.h>
/* 当前64位环境,一个线程占1024*1024*1024大小的stack,可以创建4553个,每个线程
创建一个i,取i的地址
0x7fccf3bfde44
0x7fcc6bbfbe44
0x7fcd33bfee44
0x7fcc2bbfae44
0x7fcbebbf9e44
0x7fcbabbf8e44
0x7fccb3bfce44
0x7fcb6bbf7e44
count = 4553
*/
void * func(void *p)
{
	int i ;
	printf("%p\n",&i);
	while(1)
		pause();
	pthread_exit(NULL);

}


int main()
{
	int i ,count = 0;
	int err;
	struct timespec tim;
	tim.tv_sec = 100;
	tim.tv_nsec = 0;
	pthread_t tid;
	pthread_attr_t attr;
	
	pthread_attr_init(&attr);

	pthread_attr_setstacksize(&attr, 1024*1024*1024);

	for(i = 0 ; ; i++ )
	{
		err = pthread_create(&tid, &attr,func , NULL);
		if(err)
		{
			fprintf(stderr,"pthread_create():%s\n",strerror(err));
			break;
		}
		count++;

	}


	printf("%d\n" ,count);
	nanosleep(&tim,&tim);

	pthread_attr_destroy(&attr);
	exit(0);
}

