#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int main(int argc,char **argv){
	
	FILE * fp;
	char* linebuf = NULL;
	size_t linesize = 0;
	if(argc<2){
		fprintf(stderr,"Usage:...\n");
		exit(1);
	}

	fp = fopen(argv[1],"r");
	if(fp==NULL){
		perror("fopen():");
		exit(1);
	}

	

	while(1){
		
		if(getline(&linebuf,&linesize,fp) < 0)	break;
		printf("%ld\n",strlen(linebuf));
		printf("%ld\n",linesize);
		//getline里边的*lineptr没有释放
		//用free也可能出现问题，动态内存实现的函数不只是malloc和free
		//还有其他的处理
		//就好象如果是用c++的new和delete，就不能单纯free
	}
	
	free(linebuf);
	


	fclose(fp);
	
	exit(0);
}
