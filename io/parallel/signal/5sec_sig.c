#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
//valatile表示不要轻信内存中对这个数的存放,去到真正的空间去取数值
static volatile int loop = 1; 

static void alrm_handler(int s)
{
	loop = 0;
}

int main()
{
	int64_t count = 0;

	signal(SIGALRM,alrm_handler);
	alarm(5);

	while(loop)
		count++;

	printf("%ld\n",count);


	exit(0);
}

