#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<errno.h>
#include<sys/wait.h>
#define PROCNUM		20
#define FNAME		"/tmp/out"
#define LINESIZE	1024

static int semid;

//用semaphore array 写一个互斥量 , 替代mut



static void P(void) //取资源量,
{
	struct sembuf op;//当前要操作的semphore,信号量的总的个数
	
	op.sem_num = 0; //资源下标
	op.sem_op = -1;	//减1
	op.sem_flg = 0;	//特殊要求:没有 选0


	while( semop(semid,&op,1) < 0 )	//从资源量减1,做op,资源总量
	{
		if(errno != EINTR || errno != EAGAIN)//真错,结束
		{
			perror("semop");
			exit(1);
		}
	}
}


static void V(void) //归还资源量
{

	struct sembuf op;

	op.sem_num = 0;
	op.sem_op = 1;
	op.sem_flg = 0;

	if( semop(semid,&op,1) < 0) //一般按归还很少出假错
	{
		 perror("semop()");
		 exit(1);
	}

}


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
	/* if error 判错*/
	

	P();


	fgets(linebuf,LINESIZE,fp);
	fseek(fp,0,SEEK_SET);
	sleep(1);
	fprintf(fp,"%d\n",atoi(linebuf)+1);
	fflush(fp);

	
	V();

	fclose(fp);

	return;
}

int main()
{
	int i;
	int err;
	
	pid_t pid;
	//有亲缘关系的进程间通信可以不用ftok,用一个匿名的ipc就行,IPC_PRIVATE
	//key = ftok(); 
	//fork后每一个子进程都拿到一样的key值,所以不需要ftok,直接再semget里用一个相同的key即可,想要保证不冲突还在进程内,可以使用IPC_PRIVATE,匿名IPC
	semid = semget(IPC_PRIVATE, 1 , 0600);	//IPC_PRIVATE ,互斥量的个数
	if(semid < 0)
	{
		perror("semget()");
		exit(1);
	}

	
	//初始化
	if(semctl(semid ,0,SETVAL, 1 )< 0)
	{
		perror("semctl()");
		exit(1);
	}


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

	//销毁
	semctl(semid , 0 , IPC_RMID );

	exit(0);
}

