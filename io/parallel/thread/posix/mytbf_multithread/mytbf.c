#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include "mytbf.h"
//
//static也直接初始化了
static struct mytbf_st* job[MYTBF_MAX];
static pthread_mutex_t mut_job = PTHREAD_MUTEX_INITIALIZER;
static int inited = 0;
//typedef void (*sighandler_t)(int);
static pthread_t tid_alrm;
static pthread_once_t init_once = PTHREAD_ONCE_INIT;

//static sighandler_t alrm_handler_save;

struct mytbf_st
{
	int cps;	//c per second
	int burst;	//上限
	int token;	
	int pos;	
	pthread_mutex_t mut;
	pthread_cond_t cond;
};

static int get_free_pos_unlock(void)
{

	for(int i = 0 ; i < MYTBF_MAX ; i++)
	{
		if(job[i] == NULL)
			return i;
	}
	return -1;
}



static void* thr_alrm(void *p)
{
//	alarm(1);
//
		fprintf(stderr,"pthread_create():running \n");
	struct timespec ts;
	ts.tv_sec = 1;
	ts.tv_nsec = 0;
	while(1){

		pthread_mutex_lock(&mut_job);
		for(int i = 0 ; i < MYTBF_MAX; i++)
		{
			if(job[i] == NULL)
				continue;
			pthread_mutex_lock(&job[i]->mut);
			job[i]->token += job[i]->cps;
			if(job[i]->token > job[i]->burst)
				job[i]->token=job[i]->burst;
			pthread_cond_broadcast(&job[i]->cond);
			pthread_mutex_unlock(&job[i]->mut);
		}
		pthread_mutex_unlock(&mut_job);
		nanosleep(&ts,NULL);
	}
	pthread_exit(NULL);
}
//
//unload只能一个人调用,多于一个人就可能出错,pthread_destroy了
static void module_unload(void)
{
	int i;
//	signal(SIGALRM,alrm_handler_save);
//	alarm(0);
//	回收module_load出来的thr_alrm线程
	pthread_cancel(tid_alrm);
	pthread_join(tid_alrm,NULL);
	for(i = 0 ; i < MYTBF_MAX ; i++)
	{
		if(job[i] != NULL)
		{
			mytbf_destroy(job[i]);
		}
		free(job[i]);
	}
	pthread_mutex_destroy(&mut_job);
}

static void module_load(void)
{
//	alrm_handler_save = signal(SIGALRM,alrm_handler);
//	alarm(1);
	int err;
	err = pthread_create(&tid_alrm,NULL,thr_alrm,NULL);
	if(err)
	{
		fprintf(stderr,"pthread_create():%s\n",strerror(err));
	}

	

	atexit(module_unload);
}

mytbf_t *mytbf_init(int cps,int burst)
{
	struct mytbf_st *me;
	/*
	pthread_mutex_lock();
	if(!inited)
	{
		module_load();
		inited = 1;
	}
	unlock();
	*/

	pthread_once(&init_once,module_load);

	int pos;


	me = malloc(sizeof(*me));
	if(me == NULL)
		return NULL;
	me->token = 0;
	me->cps = cps;
	me->burst = burst;
	pthread_mutex_init(&me->mut,NULL);
	pthread_cond_init(&me->cond,NULL);
	pthread_mutex_lock(&mut_job);
	pos = get_free_pos_unlock();
	if(pos < 0)
	{
		pthread_mutex_unlock(&mut_job);
		free(me);
		return NULL;
	}
	me->pos = pos;

	job[pos] = me;

	pthread_mutex_unlock(&mut_job);

	return me;
}

static int min(int a,int b)
{
	return a<b?a:b;
}

//取令牌
int mytbf_fetchtoken(mytbf_t* ptr,int size)
{
	int n = 0;
	struct mytbf_st * me = ptr;
	if(size <= 0)
		return -EINVAL;
	pthread_mutex_lock(&me->mut);
	while(me->token <= 0)
	{
		//发现没有token值,解锁,等待,等pthread_cond_signal或者broadcast
		pthread_cond_wait(&me->cond,&me->mut);
		/*
		pthread_mutex_unlock(&me->mut);
		sched_yield();
		pthread_mutex_lock(&me->mut);
		*/
	}

	n =min(me->token,size);
	me->token -= n;
	pthread_mutex_unlock(&me->mut);

	return n;

}
//多了还回去
int mytbf_returntoken(mytbf_t* ptr,int size)
{
	struct mytbf_st *me = ptr;
	if(size <= 0)
		return -EINVAL;
	pthread_mutex_lock(&me->mut);
	me->token += size;
	if(me->token > me->burst)
		me->token = me->burst;
	pthread_cond_broadcast(&me->cond);
	pthread_mutex_unlock(&me->mut);

	return size;
}


int mytbf_destroy(mytbf_t* ptr)
{
	struct mytbf_st* me = ptr;
	
	pthread_mutex_lock(&mut_job);
	job[me->pos] = NULL;
	pthread_mutex_unlock(&mut_job);

	fprintf(stderr,"berfore destroy  me->mut\n");
	pthread_mutex_destroy(&me->mut);
	pthread_cond_destroy(&me->cond);
	free(ptr);
	

	return 0;
}
