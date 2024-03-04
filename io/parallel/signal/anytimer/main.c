#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>

#include"anytimer.h"

static void f2(void *ptr)
{
	const char *p = ptr;
	printf("f2(): %s\n",p);
}
static void f1(void *ptr)
{
	const char *p = ptr;
	printf("f1(): %s\n",p);
}


int main()
{//BEGIN!END!..bbb...aaa..ccc..............
	/*
		5,f1,"aaa";
		2,f2,"bbb";
		7,f1,"ccc";
	*/
	int job1,job2,job3;
	puts("BEGIN!");
	job1 = at_addjob(5,f1,"aaa");
	if(job1 < 0)
	{
		fprintf(stderr,"at_addjob():%s\n",strerror(-job1));
	}
	job2 = at_addjob(2,f2,"bbb");
	//调用函数传参,执行完函数才能继续下一步,造成误差累计
	//改成fork一个子进程或者抛出一个线程
	if(job2 < 0)
	{
		fprintf(stderr,"at_addjob():%s\n",strerror(-job2));
	}
	job3 = at_addjob(7,f1,"ccc");
	if(job3 < 0)
	{
		fprintf(stderr,"at_addjob():%s\n",strerror(-job3));
	}
	puts("END!");

	while(1)
	{
		write(1,".",1);
		sleep(1);
	}







	exit(0);
}

