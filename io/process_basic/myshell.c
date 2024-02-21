#include<stdio.h>
#include<glob.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<string.h>
/*
 *
 *	while(1)
	{
		prompt();		//打印终端内容
		getline();		//获取命令
		parse();		//解析命令
		if(内部命令)
		{
		}
		}else if(外部命令)
		{
			fork();		//创建子进程
			if( < 0)
			{
			}
			if( ==0)	//子进程运行
			{
				execXX();
				perror();
				exit(1);
			}else			//parent
			{
				wait();		//父进程等待
			}	
		}
	}


 */
#define DELIMS	" \t\n" 

struct cmd_st
{
	glob_t globres;
};

static void prompt(void )
{
	printf("mysh-1.0$ :");
	fflush(NULL);
}

static void parse(char* line,struct cmd_st *res)
{
	char *tok;
	int i = 0;
	while(1)
	{
		tok = strsep(&line,DELIMS);
		if(tok == NULL)
			break;
		if(tok[0] == '\0')
			continue;
		
		glob(tok, GLOB_NOCHECK|GLOB_APPEND*i,NULL,&res->globres);
		i = 1;
	};
}

int main()
{
	char *linebuf = NULL;
	size_t linebuf_size = 0;
	struct cmd_st cmd;
	pid_t pid;
	while(1)
	{
		prompt();//打印终端内容

		if(getline(&linebuf,&linebuf_size,stdin)<0)//获取命令
			break;
		parse(linebuf,&cmd);//解析命令

		if(0)
		{/*do sth*/}
		else
		{
			pid = fork();
			if(pid  < 0)
			{
				perror("fork()");
				exit(1);
			}
			if( pid==0)
			{
				execvp(cmd.globres.gl_pathv[0],cmd.globres.gl_pathv);
				perror("execvp()");
				exit(1);
			}else			//parent
			{
				wait(NULL);
			}	
		}
	}

}

