#include<stdio.h>
#include<stdlib.h>

#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<string.h>

#include "proto.h"


int main()
{

	key_t key;
	key = ftok(KEYPATH,KEYPROJ);

	int msgid;

	

	struct msg_st sbuf;
	sbuf.mtype = 1;
	strcpy(sbuf.name,"Alan");
	sbuf.math = rand()%100;
	sbuf.chinese = rand()%100;

	msgid = msgget(key, 0); //get msgget
	
	if(msgid < 0)
	{
		perror("msgget()");
		exit(1);
	}
	int size;
	size = msgsnd(msgid, &sbuf, sizeof(sbuf)-sizeof(long),0); //发送 msgop
	if(size < 0)
	{
		perror("msgsnd()");
		exit(1);
	}
	
	printf("OK!\n");

	exit(0);
}

