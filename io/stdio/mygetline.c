#include<stdio.h>
#include<stdlib.h>
#include<string.h>
/*
 *
 *  有错!!!!!,详情见mygetline2.c
 *
 *
 *
 * p13作业：自主实现getline()函数
 *mygetline(char ** lineptr,size_t * n, FILE *stream);
 *实现动态申请内存，把一行的内容放到*lineptr中，malloc n个空间，不够
 *再remalloc重新申请
 *
 *mygetline_free(char ** lineptr,size_t *n,FILE * stream)
 *把lineptr里的内容free了，
 *
 */

ssize_t mygetline(char** lineptr , size_t *n,FILE * stream){
	//set buf as a container to caontain one line of character
	//lineptr is a pointer to a char*(one buf)
	char *buf = *lineptr;
	ssize_t c , i = 0;

	
	
	//刚开始为*n赋值为0,并分配空间
	if(buf == NULL || *n == 0 ){
		*lineptr = malloc(10);
		buf = *lineptr;
		*n = 10;
	}
	//fgetc获取当前字符并放到*buf里，2种可能退出循环
	//1.文件末尾
	//fgetc返回EOF表示文末，后面没有内容，退出
	//2.读取
	//读到'\n'，表示读取该行完成
	//
	//读取过程两种现象
	//1.正常读取：i <*n - 2 
	//直接把读出的字符放到*(buf+i)即可
	//2.需要重新realloc内存
	//
	while((c = fgetc(stream)) != '\n'){
		if(c == EOF) return -1;
		if( i < *n-2 ){
			*(buf+i) = c;
			i++;
		}else{
			*n = *n +10;
			buf = realloc(buf , *n);
			*(buf + i ) = c;
			i++;
		}
	}
	*(buf+i) = '\n';
	*(buf+i+1) = '\0';
	return i;
}

void mygetline_free(char * lineptr ){
	free(lineptr);
	lineptr = NULL;	
}

int main(int argc,char** argv){
	char * linebuf = NULL;
	size_t n = 0;
	FILE * fps,*fpd;
	fps = fopen(argv[1],"r");
	if(fps == NULL){
		perror("fopen():");
		exit(1);
	}
	fpd = fopen(argv[2],"w");
	if(fpd == NULL){
		perror("fopen():");
		exit(1);
	}
	while(1){
		if(mygetline(&linebuf,&n,fps) < 0) break;
		printf("%ld\n",strlen(linebuf));
		printf("%ld\n",n);		
		fputs(linebuf , fpd);

	}

//	mygetline_free(linebuf);
//	free(linebuf);
	fclose(fps);
	fclose(fpd);
	exit(0);
}
