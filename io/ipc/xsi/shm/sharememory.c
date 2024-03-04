#include<stdio.h>
#include<sys/types.h>
#include<stdlib.h>
#include<sys/ipc.h>
#include<sys/wait.h>
#include<unistd.h>
#include<sys/shm.h>
#include<string.h>
#define MEMSIZE		1024

int main()
{
	//ftok();
	char * ptr;
	int shmid;
	pid_t pid;

	shmid = shmget(IPC_PRIVATE, MEMSIZE , 0600 );	//key_t key,要设置的共享内存的size大小,shmflg特殊要求,
						//由于用了IPC_PRIVATE,需要给出创建的权限
	if(shmid < 0)
	{
		perror("shmid()");
		exit(1);
	}
	

	pid = fork();
	if(pid < 0)
	{
		perror("fork");
		exit(1);
	}
	if( pid == 0 ) 	//child write
	{
		ptr = shmat(shmid , NULL , 0);
		if(ptr ==(void *) -1)
		{
			perror("shmat()");
			exit(1);
		}

		strcpy(ptr, "Hello!" );
		
		sleep(10);
		shmdt(ptr);//一般认为解除映射不会失败
		exit(0);

	}
	//parent read
	wait(NULL);
	ptr = shmat(shmid , NULL , 0);//映射到自己空间里
	if(ptr == (void* ) -1)
	{
		perror("shmat()");
		exit(1);
	}
	puts(ptr);
	shmdt(ptr);

	shmctl(shmid , IPC_RMID , NULL);//父进程销毁shmid
	exit(0);




	exit(0);
}

