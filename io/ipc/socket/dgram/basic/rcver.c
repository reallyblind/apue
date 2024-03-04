#include<stdio.h>
#include<stdlib.h>

#include "proto.h"

int main()
{
	int sd;
	sd = socket(AF_INET,SOCK_DGRAM,0/*IPPROTO_UDP*/);//每个协议族当中,对一种实现方式的支持,一定有一个是默认的
					 //此处指 用AF_INET协议族当中的默认支持SOCK_DGRAM报式套接字的那个协议来实现

	if(sd < 0) //文件描述符不可能小于0
	{
		perror("socket");
		exit(1);
	}

	struct sockaddr_in laddr;

	laddr.sin_family = AF_INET;
	laddr.in_port_t = htons(atoi(RCVPORT));
	inet_pton(AF_INET, "0.0.0.0" , &laddr.sin_addr);//0.0.0.0会换成当前的ip地址
	if ( bind(sd , (void * )&laddr , sizeof(laddr)) < 0)
	{
		perror("bind()");
		exit(1);
	}

	while(1)
	{
		recvfrom();
		printf();
	}


	close();
	

	exit(0);
}

