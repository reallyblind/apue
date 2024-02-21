#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define BUFSIZE 5
int main(int argc,char**argv){
	FILE *fps,*fpd;
	char buf[BUFSIZE];
	
	if(argc<3){
		fprintf(stderr,"Usage:%s <src_file> <dest_file>\n",argv[0]);
		exit(1);
	}

	fps = fopen(argv[1],"r");
	if(fps==NULL){
		perror("fopen():");
		exit(1);
	}

	fpd = fopen(argv[2],"w");
	if(fpd == NULL){
		fclose(fps);
		perror("fopen():");
		exit(1);
	}
	int count = 0;
	while(fgets(buf,BUFSIZE,fps) != NULL)
	{
		fputs(buf , fpd);
		printf("buf length %ld\n",strlen(buf));
		count ++;
	}
	printf("%d",count);	
	fclose(fpd);
	fclose(fps);



	exit(0);
}
