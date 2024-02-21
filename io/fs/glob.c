#include<stdio.h>
#include<stdlib.h>
#include<glob.h>

#define PAT "/etc/a*.conf"
//#define PAT "/etc/.*"



int main(int argc,char **argv)
{
	glob_t globres;
	int err;
	err = glob(PAT,0,NULL,&globres);
	if(err)
	{
		printf("Error code = %d\n",err);
		exit(1);
	}

	for(int i = 0 ; i < globres.gl_pathc ; i++ )
		puts(globres.gl_pathv[i]);

	globfree(&globres);



	exit(0);
}

