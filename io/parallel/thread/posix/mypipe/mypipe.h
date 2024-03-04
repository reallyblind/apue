#ifndef MYPIPE_H__
#define MYPIPE_H__
//管道特点:单功的,一方读一方写,首部读尾部写
//		要有读者和写者

//建议再加一层封装,把身份记录,防止用户注册的是读者,但一直在写
//struct fenzhuang
//{
//	struct mypipe_st *ptr;
//	int opmap;
//}
//open() ---  mypipe_init & mypipe_register
//read() ---  mypipe_read
//write() --- mypipe_write
//close() --- mypipe_destroy & mypipe_unregister

//类似文件描述符fd

#define PIPESIZE	1024
#define MYPIPE_READ	0x00000001UL
#define MYPIPE_WRITE	0x00000002UL

typedef void mypipe_t;


mypipe_t *mypipe_init(void);

int mypipe_register(mypipe_t *,int opmap);

int mypipe_unregister(mypipe_t *,int opmap);

int mypipe_read(mypipe_t *,void * buf,size_t count);

int mypipe_write(mypipe_t* ,const void *buf,size_t size);

int mypipe_destroy(mypipe_t *);



#endif
