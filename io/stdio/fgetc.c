#include<stdio.h>
#include<stdlib.h>
//获取文件字符个数
int main(int argc,char** argv){
	int count = 0;
	FILE * fp;
	if(argc<2){
		fprintf(stderr,"Usage:... \n");
		exit(1);
	}
	
	fp = fopen(argv[1],"r");
	if(fp == NULL){
		perror("fopen():");
		exit(1);
	}
	while(fgetc(fp) != EOF){
		count++;
	}
	printf("count = %d\n",count);

	fclose(fp);


	exit(0);
}
