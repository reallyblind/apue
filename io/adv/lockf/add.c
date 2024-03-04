#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

#define PROCNUM		20
#define FNAME		"/tmp/out"
#define LINESIZE	1024


static void func_add(void )
{
	FILE * fp;
	char linebuf[LINESIZE];
	fp = fopen(FNAME,"r+");
	if(fp == NULL)
	{
		perror("fopen()");
		exit(1);
	}
	
	int fd;
	fd = fileno(fp);
	/* if error 判错*/
	lockf(fd,F_LOCK , 0);//0:表示有多长锁多长,以后再加也会锁

	fgets(linebuf,LINESIZE,fp);
	fseek(fp,0,SEEK_SET);
	sleep(1);
	fprintf(fp,"%d\n",atoi(linebuf)+1);
	fflush(fp);

	lockf(fd,F_ULOCK,0);

	fclose(fp);

	return;
}

int main()
{
	int i;
	int err;
	
	pid_t pid;


	for(i = 0 ; i< PROCNUM ; i++)
	{
		pid = fork();
		if(pid < 0)
		{
			perror("fork()");
			exit(1);
		}
		if(pid == 0)
		{
			func_add();
			exit(0);
		}
	}
	
	for( i = 0 ; i < PROCNUM ; i++)
		wait(NULL);
	

	exit(0);
}

