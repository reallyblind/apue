#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
/*
 *	date +%s
 *
 *
 *
 */



int main()
{
	puts("Begin()!");
	fflush(NULL);
	execl("/bin/date","date","+%s",NULL);
	perror("execl()");
	exit(1);

	puts("end()");

	exit(0);
}

