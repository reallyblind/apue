#include"anytimer.h"
#include<errno.h>
#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#define ANYTIMER_MAX	1024

#define PAUSE		0
#define RUNNING		1
#define OVERED		2

static struct anytimer* job[ANYTIMER_MAX];
typedef void (*sighandler_t)(int);
static sighandler_t alrm_handler_save;
struct anytimer
{
	int clock;	
	at_jobfunc_t *func;
	void *data;
	int stat;
	int pos;
};
static int inited = 0;


static void module_unload(void )
{
	int i ;
	signal(SIGALRM,alrm_handler_save);
	alarm(0);
	for( i = 0 ; i < ANYTIMER_MAX ; i++)
	{
		free(job[i]);
		job[i] = NULL;
	}
}
static int get_free_pos(void)
{
	int i;
	for(i = 0 ; i < ANYTIMER_MAX; i++)
	{
		if(job[i] == NULL || job[i]->stat == OVERED)
			return i;
	}
	return -1;
}
void alrm_handler(int s)
{
	alarm(1);
	for(int i = 0 ; i < ANYTIMER_MAX ;i ++)
	{
		if(job[i] == NULL)
			continue;
		if(job[i]->stat != RUNNING)
			continue;
		if(job[i]->clock-1 > 0)
		{
			job[i]->clock--;
			continue;
		}
		job[i]->clock = 0;
		job[i]->func(job[i]->data);
		job[i]->stat = OVERED;
	}
}

static void module_load(void )
{
	alrm_handler_save = signal(SIGALRM,alrm_handler);
	alarm(1);

	atexit(module_unload);
}

int at_addjob(int sec,at_jobfunc_t *jobp,void *arg)
{
	if(sec <= 0)
		return -EINVAL;
	struct anytimer * me;
	if(!inited){
		module_load();
		inited = 1;
	}
	int i,j;
	i = get_free_pos();
	if(i == -1)
		return -ENOSPC;
	me = malloc(sizeof(*me));
	if(me == NULL)
		return -ENOMEM;
		
	me->clock = sec;
	me->func = jobp;
	me->data = arg;
	me->stat = RUNNING;

	if(job[i] != NULL)
		free(job[i]);
	job[i] = me;
	return 0;
}

int at_canceljob(int id)
{
	if(job[id] == NULL)
		return -1;
	job[id]->stat = PAUSE;
	return 0;
}

int at_waitjob(int id)
{

}
