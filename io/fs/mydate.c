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

//先实现了M , m,S,d,然后实现了y和H,最后实现了非选项的传参

int main(int argc,char **argv)
{
	FILE * fp = stdout;
	struct tm *tm;
	time_t stamp;
	char fmtstr[FMTSTRSIZE];
	char timestr[TIMESTRSIZE];
	int c;
	stamp = time(NULL);
	fmtstr[0] = '\0';
	tm = localtime(&stamp);
	
	while(1)
	{
		c = getopt(argc,argv,"-H:MSy:md");
		if( c < 0)
			break;
		
		switch(c)
		{
			case 1:
				//先入为主
				if(fp == stdout)
				{
					fp = fopen(argv[optind-1],"w");
					if(fp == NULL)
					{
						perror("fopen()");
						fp = stdout;
					}
				}
				break;
			case 'H':
				if(strcmp(optarg,"12") == 0)
					strncat(fmtstr,"%I(%P) ",FMTSTRSIZE-1);
				else if(strcmp(optarg,"24") ==0)
					strncat(fmtstr,"%H ",FMTSTRSIZE-1);
				else 
					fprintf(stderr,"Invalid argument\n");
				break;
			case 'M':
				strncat(fmtstr,"%M ",FMTSTRSIZE-1);
				break;
			case 'S':
				strncat(fmtstr,"%S ",FMTSTRSIZE-1);
				break;
			case 'y':
				if(strcmp(optarg,"2")==0)
					strncat(fmtstr,"%y ",FMTSTRSIZE-1);
				else if(strcmp(optarg,"4") == 0)
					strncat(fmtstr,"%Y ",FMTSTRSIZE-1);
				else 
					fprintf(stderr,"Invalid argument of -y\n");
				break;
			case 'm':
				strncat(fmtstr,"%m ",FMTSTRSIZE-1);
				break;
			case 'd':
				strncat(fmtstr,"%d ",FMTSTRSIZE-1);
				break;
			default:
				break;
		}
	}
	

	strncat(fmtstr,"\n",FMTSTRSIZE-1);
	strftime(timestr,TIMESTRSIZE,fmtstr,tm);
	fputs(timestr,fp);
	
	if(fp != stdout)
		fclose(fp);



	exit(0);
}

