#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<fcntl.h>
#include<errno.h>
#include<poll.h>
#include<sys/stat.h>
#define BUFFSIZE	1024
#define TTY1	"/dev/tty11"
#define TTY2	"/dev/tty12"


//有限状态机,画图写好用,要把图和代码结合起来
//见图,有限状态机
enum
{
	STATE_R = 1,	//读态
	STATE_W,	//写态
STATE_AUTO,
	STATE_Ex,	//异常态
	STATE_T		//

};

struct fsm_st//有限状态机finite state machine
{
	int state;
	int sfd;
	int dfd;
	int len;
	char buf[BUFFSIZE];
	char *errstr;
	int pos;
};

static void fsm_driver(struct fsm_st *fsm)
{
	int ret;
	switch(fsm->state)
	{
		case STATE_R:
			//2021 Linux设置非阻塞方式，无法用read直接读，听说必须用select判断先，
			fsm->len = read(fsm->sfd,fsm->buf,BUFFSIZE);
			if(fsm->len == 0)
			{
				fsm->state = STATE_T;
			}
			else if(fsm->len < 0)
			{
				if(errno == EAGAIN)
					fsm->state = STATE_R;
				else
				{
					fsm->errstr = "read()";
					fsm->state = STATE_Ex;
				}
			}else
			{
				fsm->pos = 0;
				fsm->state = STATE_W;
			}
			break;
		case STATE_W:
			ret = write(fsm->dfd,fsm->buf+fsm->pos,fsm->len);
			if(ret < 0)
			{
				if(errno == EAGAIN)
					fsm->state = STATE_W;
				else
				{
					fsm->errstr ="write()";
					fsm->state = STATE_Ex;
				}
			}else
			{
				fsm->pos += ret;
				fsm->len -= ret;
				if(fsm->len == 0)
					fsm->state = STATE_R;
				else
					fsm->state = STATE_W;
			}
			break;
		case STATE_Ex:
			perror(fsm->errstr);
			fsm->state = STATE_T;
			break;
		case STATE_T:
			/*do sth*/
			break;
		default:
			abort();
			break;	
	}


}

static int max(int a,int b)
{
	return a>b?a:b;
}

static void relay(int fd1,int fd2)//把文件描述符修改成可用的,然后退出函数时恢复
{
	int fd1_save,fd2_save;
	struct fsm_st fsm12,fsm21;//12读1写2(左写右)  21读2写1
	struct pollfd pfd[2];

	fd1_save = fcntl(fd1,F_GETFL);
	fcntl(fd1,F_SETFL,fd1_save|O_NONBLOCK);

	fd2_save = fcntl(fd2,F_SETFL);
	fcntl(fd2,F_SETFL,fd2_save|O_NONBLOCK);

	fsm12.state = STATE_R;
	fsm12.sfd = fd1;
	fsm12.dfd = fd2;
	fsm21.state = STATE_R;
	fsm21.sfd = fd2;
	fsm21.dfd = fd1;

	pfd[0].fd = fd1;
	pfd[1].fd = fd2;

	while(fsm12.state != STATE_T || fsm21.state != STATE_T)
	{
		//布置监视任务

		pfd[0].events = 0;

		if(fsm12.state == STATE_R)
			pfd[0].events |= POLLIN;
		if(fsm21.state == STATE_W)
			pfd[0].events |= POLLOUT;
		pfd[1].events = 0;
		if(fsm21.state == STATE_R)
			pfd[1].events |= POLLOUT;
		if(fsm12.state == STATE_W)
			pfd[1].events |= POLLIN;
		
		//监视
		if(fsm12.state < STATE_AUTO ||fsm21.state < STATE_AUTO)
		{
			while(poll(pfd,2,-1) < 0)
			{
				if(errno = EINTR)
					continue;
				perror("poll()");
				exit(1);
			}
		}
		
		
		
		//查看监视结果

		if( pfd[0].revents & POLLIN || pfd[1].revents & POLLOUT \
			      	|| fsm12.state > STATE_AUTO)
			fsm_driver(&fsm12);
		if( pfd[1].revents & POLLIN || pfd[0].revents & POLLOUT \
			       	|| fsm21.state > STATE_AUTO)
			fsm_driver(&fsm21);

	}
	



	fcntl(fd1,F_SETFL,fd1_save);
	fcntl(fd2,F_SETFL,fd2_save);
}

int main()
{
	int fd1,fd2;

	fd1 = open(TTY1,O_RDWR);
	if(fd1 < 0)
	{
		perror("fd1 open():");
		exit(1);
	}
	write(fd1,"TTY1\n",5);


	fd2 = open(TTY2,O_RDWR|O_NONBLOCK);
	if(fd2 < 0)
	{
		perror("fd2 open():");
		exit(1);
	}
	write(fd2,"TTY2\n",5);

	relay(fd1,fd2);

	close(fd1);
	close(fd2);

	exit(0);
}
