#include<stdio.h>
#include<stdlib.h>

int main(){
	char buf[1024];
	int year = 2014 , month = 5, day = 13;

	printf("%d-%d-%d\n",year,month,day);//伪装出来的2014-5-13
	
	sprintf(buf,"%d-%d-%d",year,month,day);//把后面内容放到buf中
	puts(buf);

	//char str[] = "123a456";
	//printf("%d\n",atoi(str));//123

	exit(0);
}
