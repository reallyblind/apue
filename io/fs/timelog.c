#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>

#define BUFFSIZE	1024

int main(int argc,char **argv)
{
	struct tm *tm;
	FILE * fp;
	char buf[BUFFSIZE];
	int count = 0;
	if(argc < 2)
	{
		fprintf(stderr,"Usage..\n");
		exit(1);
	}
	time_t stamp;
	fp = fopen(argv[1],"a+");
	if(fp == NULL)
	{
		perror("fopen()");
		exit(1);
	}



	while(fgets(buf,BUFFSIZE,fp) != NULL)
		count++;

	while(1)
	{
		time(&stamp);
		tm = localtime(&stamp);
		fprintf(fp,"%-4d%d-%d-%d %d:%d:%d\n",++count,\
				tm->tm_year+1900,tm->tm_mon+1,tm->tm_mday,\
				tm->tm_hour,tm->tm_min,tm->tm_sec);
		fflush(fp);
		//strftime(fp,BUFFSIZE,"%Y-%m-%d %H:%M:%S",tm);
		sleep(1);
	}

	
	fclose(fp);

	exit(0);
}



