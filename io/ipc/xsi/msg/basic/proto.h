//约定协议,这就是制作协议

//当前协议只是同一台机器下所以很多东西可以忽略,对齐方式是否相同,机器字长是否相同,数据结构是否相同等均可以忽略

#ifndef PROTO_H__
#define PROTO_H__

#define KEYPATH		"/etc/services"
#define KEYPROJ		'g'	//ansca码确保是一个整形

#define NAMESIZE	32

struct msg_st
{
	long mtype;
	char name[NAMESIZE];
	int math;
	int chinese;
};













#endif

