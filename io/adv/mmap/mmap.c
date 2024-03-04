#include<stdio.h>
#include<stdlib.h>
#include<sys/mman.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>


int main(int argc,char** argv)
{

	if(argc < 2 )
	{
		fprintf(stderr,"Usage...\n");
		exit(1);
	}
	int fd;
	struct stat statres;
	char* str;
	int i , count;
	fd = open(argv[1],O_RDONLY);
	if(fd < 0)
	{
		perror("open()");
		exit(1);
	}
	
	
	//stat();
	if(fstat(fd,&statres) < 0)
	{
		perror("fstat()");
		exit(1);
	}



	str = mmap(NULL,statres.st_size,PROT_READ,MAP_SHARED,fd,0);
	if(str == MAP_FAILED)
	{
		perror("mmap()");
		exit(1);
	}
	//已经把一块硬盘映射到存储空间了,可以关闭了
	close(fd);
	
	for( i = 0 ; i < statres.st_size ; i++)
	{
		if(str[i] == 'a')
			count++;
	}

	printf("count = %d\n",count);


	munmap(str,statres.st_size);


	exit(0);
}

