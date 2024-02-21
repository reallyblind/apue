#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>
#include<glob.h>
#define PATHSIZE 	1024

//path为非目录文件 st_blocks/2
//path为目录文件,展开
//	path/*
//	glob();
//	stat();
//
//	path/.*
//	glob();
//

static int path_noloop(const char *path)
{
	char *pos;
	pos = strrchr(path,'/');
	if(pos == NULL)
		exit(1);
	if(strcmp(pos+1,".") == 0 || strcmp(pos+1,"..")==0)
		return 0;
	return 1;

}

//递归优化,可以把递归前的变量设置为静态区static,其他的只能用auto

static int64_t mydu(const char* path)
{
	static struct stat statres;
	static char nextpath[PATHSIZE];
	int ret = 0;
	glob_t globres;
	int i  = 0;
	int64_t sum = 0;
	if(lstat(path,&statres)<0)
	{
		perror("lstat()");
		exit(1);
	}

	if(!S_ISDIR(statres.st_mode))
		return statres.st_blocks;

	strncpy(nextpath,path,PATHSIZE);
	strncat(nextpath,"/*",PATHSIZE-1);
	ret = glob(nextpath , 0 , NULL , & globres);
	switch (ret)
	{
		case GLOB_NOSPACE:
			fprintf(stderr, " no space\n");
			break;
		case GLOB_ABORTED:
			fprintf(stderr,"read error\n");
			break;
		case GLOB_NOMATCH:	
			fprintf(stderr,"no found match\n");
			break;
	}
	
	strncpy(nextpath,path,PATHSIZE);
	strncat(nextpath,"/.*",PATHSIZE-1);
	ret = glob(nextpath, GLOB_APPEND , NULL , &globres);
	switch (ret)
	{
		case GLOB_NOSPACE:
			fprintf(stderr, " no space\n");
			break;
		case GLOB_ABORTED:
			fprintf(stderr,"read error\n");
			break;
		case GLOB_NOMATCH:	
			fprintf(stderr,"no found match\n");
			break;
	}
	sum += statres.st_blocks;

	for(i = 0 ; i < globres.gl_pathc ; i++)
		if(path_noloop(globres.gl_pathv[i]))
		sum += mydu(globres.gl_pathv[i]);	

	globfree(&globres);
	return sum;

}


int main(int argc,char** argv)
{
	if(argc < 2 )
	{
		fprintf(stderr,"Usage...\n");
		exit(1);
	}

	printf("%ld\n",mydu(argv[1])/2);




	exit(0);
}
