#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<unistd.h>
#define TIMESTRSIZE	1024

/*
 *	-y: year
 *	-m: month
 *	-d: day
 *	-H: hour
 *	-M: minute
 *	-S: second
 *
 */
#define FMTSTRSIZE	1024

int main(int argc,char **argv)
{
	struct tm *tm;
	time_t stamp;
	char timestr[TIMESTRSIZE];
	int c;
	stamp = time(NULL);
	tm = localtime(&stamp);
	tm->tm_mday += 100;
	(void)mktime(tm);
	strftime(timestr,TIMESTRSIZE,"100 days later:%Y-%m-%d %H:%M:%S",tm);
	
	puts(timestr);
	
	



	exit(0);
}

