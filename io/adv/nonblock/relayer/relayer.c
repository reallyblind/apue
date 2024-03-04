#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<fcntl.h>
#include<string.h>
#include<errno.h>
#include<sys/stat.h>
#include<pthread.h>

#include "relayer.h"

//有限状态机,画图写好用,要把图和代码结合起来
//见图,有限状态机
enum
{
	STATE_R = 1,	//读态
	STATE_W,	//写态
	STATE_Ex,	//异常态
	STATE_T		//

};

#define BUFFSIZE	1024

struct rel_fsm_st//有限状态机finite state machine
{
	int state;
	int sfd;
	int dfd;
	int len;
	char buf[BUFFSIZE];
	char *errstr;
	int pos;
	int64_t count;
};

struct rel_job_st//实际的中继器数据
{
	int job_state;
	int fd1;
	int fd2;
	struct rel_fsm_st fsm12,fsm21;
	//struct timerval start,end;
	int fd1_save,fd2_save;

};

static struct rel_job_st  * rel_job[REL_JOBMAX];
static pthread_mutex_t mut_rel_job = PTHREAD_MUTEX_INITIALIZER;
static pthread_once_t init_once = PTHREAD_ONCE_INIT;

int get_free_pos_unlock()
{
	for(int i = 0 ; i < REL_JOBMAX ; i++)
	{
		if(rel_job[i] == NULL)
			return i;
	}
	return -1;
}
static void fsm_driver(struct rel_fsm_st *fsm)
{
	int ret;
	switch(fsm->state)
	{
		case STATE_R:
			//2021 Linux设置非阻塞方式，无法用read直接读，听说必须用select判断先，
			fsm->len = read(fsm->sfd,fsm->buf,BUFFSIZE);
			if(fsm->len == 0)
				fsm->state = STATE_T;
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


static void* thr_relayer(void * )
{
	int i;
	while(1)
	{
		pthread_mutex_lock(&mut_rel_job);
		for(i = 0 ; i < REL_JOBMAX; i++ )
		{
			if(rel_job[i] == NULL)
				continue;
			if(rel_job[i]->fsm12.state == STATE_T && rel_job[i]->fsm21.state == STATE_T)
				continue;
			if(rel_job[i]->job_state == STATE_RUNNING)
			{
				fsm_driver(&rel_job[i]->fsm12);
				fsm_driver(&rel_job[i]->fsm21);
				if(rel_job[i]->fsm12.state == STATE_T && 
					rel_job[i]->fsm21.state == STATE_T)
					rel_job[i]->job_state = STATE_OVER;
			}
		}

		pthread_mutex_unlock(&mut_rel_job);
	}
}

static void module_load(void)
{
	pthread_t tid_relayer;
	int err;
	err = pthread_create(&tid_relayer,NULL,thr_relayer,NULL);

	if(err)
	{
		fprintf(stderr,"pthread_create():%s\n",strerror(err));
		exit(1);
	}


}

//module_unload();
int rel_addjob(int fd1,int fd2)
{
	struct  rel_job_st *me;
	int pos;
	pthread_once(&init_once,module_load);

	me = malloc(sizeof(*me));
	if(me == NULL)
		return -ENOMEM;
	


	me->fd1 = fd1;
	me->fd2 = fd2;
	me->job_state = STATE_RUNNING;
	//保证非阻塞读取
	me->fd1_save = fcntl(me->fd1,F_GETFL);
	fcntl(me->fd1,F_SETFL,me->fd1_save|O_NONBLOCK);
	me->fd2_save = fcntl(me->fd2,F_GETFL);
	fcntl(me->fd2,F_SETFL,me->fd2_save|O_NONBLOCK);
	
	me->fsm12.sfd = me->fd1;
	me->fsm12.dfd = me->fd2;
	me->fsm12.state = STATE_R;

	me->fsm21.sfd = me->fd2;
	me->fsm21.dfd = me->fd1;
	me->fsm21.state = STATE_R;
	pthread_mutex_lock(&mut_rel_job);
	pos = get_free_pos_unlock();
	if(pos < 0)
	{
		pthread_mutex_unlock(&mut_rel_job);
		fcntl(me->fd1,F_SETFL,me->fd1_save);
		fcntl(me->fd2,F_SETFL,me->fd2_save);
		free(me);
		return -ENOSPC;
	}
	rel_job[pos] = me;
	
	pthread_mutex_unlock(&mut_rel_job);
	return pos;

}



#if 0
int rel_canceljob(int )
int rel_waitjob(int , struct rel_stat_st * )
int rel_statjob(int id, struct rel_stat_st *)
#endif
