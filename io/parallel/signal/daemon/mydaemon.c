#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<syslog.h>
#define FNAME "/tmp/out"
#include<errno.h>
#include<signal.h>
static	FILE *fp;
static int daemonize(void)
{
	int fd;
	pid_t pid;
	pid = fork();
	if(pid < 0)	//父进程退出,子进程来进行setsid创建新的session,成为进程组的组长,session的组长.如果还把标准终端关闭,则成为守护进程
	{
		perror("fork()");
		return -1;
	}
	if(pid > 0)
		exit(0);
	fd = open("/dev/null",O_RDWR);	//进行脱离控制终端
	if(fd < 0)
	{
		perror("open()");
		return -1;
	}
	dup2(fd,0);	//脱离控制终端
	dup2(fd,1);
	dup2(fd,2);

	if(fd > 2)
		close(fd);

	setsid();

	chdir("/");
	umask(0);

	return 0;
}

static void daemon_exit(int s)
{

	fclose(fp);
	closelog();
	exit(0);
}

int main()
{
	int i;
	struct sigaction sa;
	sa.sa_handler = daemon_exit;
	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask,SIGQUIT);
	sigaddset(&sa.sa_mask,SIGTERM);
	sigaddset(&sa.sa_mask,SIGINT);

	sa.sa_flags = 0;
	//这就是sigaction的sa_mask的特点了，在执行处理函数前会阻塞这几个信号，就不会多次重入处理
	sigaction(SIGINT , &sa, NULL);
	sigaction(SIGQUIT,&sa, NULL);
	sigaction(SIGQUIT,&sa, NULL);

	//如果先来个SIGINT,然后SIGTERM,可能出现fclose(fp)两次,或者free两次同一空间造成内存泄露
//	signal(SIGINT,daemon_exit);
//	signal(SIGQUIT,daemon_exit);
//	signal(SIGTERM,daemon_exit);

	openlog("mydaemon",LOG_PID,LOG_DAEMON);

	if(daemonize())
	{
		syslog(LOG_ERR,"daemonize error");
		exit(1);
	}else
	{
		syslog(LOG_INFO,"daemonize() successed!");
	}
	fp = fopen(FNAME,"w");
	if(fp == NULL)
	{
		syslog(LOG_ERR,"fopen:%s",strerror(errno));
		exit(1);
	}

	syslog(LOG_INFO,"%s was opended",FNAME);

	for(i = 0 ;i < 1000 ;i++)
	{
		fprintf(fp,"%d\n",i);
		fflush(fp);
		sleep(1);
	}
	

}

