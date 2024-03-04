#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/mman.h>
#include<sys/types.h>
#include<unistd.h>
#include<wait.h>

#define MEMSIZE		1024
int main(int argc,char **argv)
{

	pid_t pid;
	char *ptr;
	//子写父读,但仍然要先都要,-1,表明只是用近似malloc的功能
	ptr = mmap(NULL,MEMSIZE,PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS,-1,0);

	

	pid = fork();
	if(pid < 0)
	{
		perror("fork()");
		munmap(ptr, MEMSIZE);
		exit(1);
	}
	if(pid == 0) //child write
	{
		strcpy(ptr,"Hello!");
		munmap(ptr,MEMSIZE);
		exit(0);
	}
	else		//parent read
	{
		wait(NULL);
		puts(ptr);
		munmap(ptr,MEMSIZE);
		exit(0);
	}

	exit(0);
}

