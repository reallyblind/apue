#include<stdio.h>
#include<stdlib.h>

#include<sys/types.h>
#include<dirent.h>
#define PAT	"/etc"

int main()
{
	DIR * dir = NULL;
	struct dirent *cur;
	dir = opendir(PAT);
	if(dir == NULL)
	{
		perror("opendir()");
		exit(1);
	}
	
	while((cur = readdir(dir)) != NULL)
	{
		puts(cur->d_name);
	}

	closedir(dir);

	exit(0);
}


