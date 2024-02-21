#include<stdio.h>
#include<stdlib.h>
/*
 *
 *缓冲区作用：大多数情况下好事，合并系统调用
 *
 *行缓冲：换行时候刷新，满了的时候刷新，强制刷新fflush。 例子：stdout，
 *	因为是终端设备
 *
 *全缓冲：满了的时候刷新，强制刷新。 例子：默认，只要不是终端设备
 *
 *无缓冲：如stderr，直接输出，需要立即输出的内容
 *
 *
 *   setvbuf:可以更改缓冲类型，一般别用
 *
 */
int main(){

	int i;


	printf("Before while()");
	fflush(stdout);
	while(1);
	
	printf("After while()");
	fflush(NULL);
	exit(0);

}
