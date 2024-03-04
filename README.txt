
			linux_c系统开发学习笔记
p15

errno : /usr/include/asm-generic/errno.h

perror("fopen():");  return type : void perror(const char *s);
fopen(): No such file or directroy
strerror(error); return type:char *strerror(int errnum);
fprintf(sterror,"fopen():%s",sterror(errno));



遇到函数返回值是一个指针的时候要问一句，这个指针指向的是静态区的地址还是堆的地址
I/O : input & output ,是一切实现的基础
	stdio，标准IO
	sysio，系统调用IO

/****************************************************************************/

I/O部分:见IO.png

			一、     stdio:标准I/O


FILE类型贯穿始终

fopen();
FILE *fopen(const char *pathname, const char *mode);
新建文件权限 : 0666 & ~umask 
$umask
0002
000 110 110 110 & 111 111 111 101
000 110 110 101 即消掉了最后一位110的中间的1,rwx,把w权限去了 

fclose();
int fclose(FILE *stream);
遇到函数返回值是一个指针的时候要问一句，这个指针指向的是静态区的地址还是堆的地址
//字符
fgetc();
int fgetc(FILE *stream);
fputc();
int fputc(int c, FILE *stream);

mycpy.c


//字符串
fgets();
fputs();

//二进制
//很多人喜欢用
fread();
fwrite();

size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
从stream取,取nmemb次,一次取size大小,放到ptr中,可以取结构体
size_t fwrite(const void *ptr, size_t size, size_t nmemb,
                     FILE *stream);


printf();//放到标准输出中
scanf();//绝对不能用%s，你不知道stdin的内容有多大，会不会把存不下

文件位置指针:
fseek();标准c中，对long的大小没定义，
	由于offset可正可负，long型，所以可正2^31=2G或负-2^31-1 = 2G-1
ftell();ftell()的返回值是long，想用的话当前文件不能超到2G
rewind();
c89,c99都支持
CONFORMING TO
       POSIX.1-2001, POSIX.1-2008, C89, C99.


fseeko();把fseek()的long改为了off_t; 
ftello();把ftell()的long改为了off_t;
	方言：c89,c99都不支持
	CONFORMING TO
       POSIX.1-2001, POSIX.1-2008, SUSv2.

	On  some architectures, both off_t and long are 32-bit types, but
       defining _FILE_OFFSET_BITS with the value  64  (before  including
       any header files) will turn off_t into a 64-bit type.

文件流刷新
fflush();

getline();


临时文件 ：  1、 如何不冲突      2、及时销毁
	char *tmpnam(char *s) : 创建一个临时文件需要两条指令，产生名字，fopen创建文件,分为两步,可能会让两个用户对同一个文件操作,产生冲突
	FILE *tmpfile(void): create a temporary file，直接产生一个二进制文件读写模式，产生了一个文件，但是ls看不见，在磁盘上产生了但是看不见的：匿名文件，不会冲突。 并且可以直接fclose()直接销毁。即使忘记fclose()正常return 0;后就会销毁file,是一个可控的内存泄漏（该进程不一直运行）。如果只打开不关闭会出现打开的流达到1024无法继续打开。


/******************************************************************************/

			二、sysio 文件IO / 系统调用IO

fd(file descript)文件描述符是在文件IO中贯穿始终的类型

1.文件描述符号的概念
（整形数，数组下标,文件描述符优先使用当前可用范围内最小的，先把数组前几位用满再用后面的）
	p15反复看,p135
	见图片linux_c_io.png

	buff是用户态读写数据暂存区，cache是内核态读写加速机制,
	buff是加速写的,cache是加速读的
	
2.文件描述符的使用/文件IO操作：open,close,read,write,lseek
	r -> O_RDONLY
	r+-> O_RDWR
	w -> O_WRONLY | O_CREAT | O_TRUNC
	w+-> O_RDWR | O_TRUNC | O_CREAT
	
	int open(const char *pathname, int flags);
	int open(const char *pathname, int flags, mode_t mode);
	int close(int fd);一般默认close不会出错
	ssize_t read(int fd, void *buf, size_t count);
	ssize_t write(int fd, const void *buf, size_t count);
	off_t lseek(int fd, off_t offset, int whence);


	int fileno()

3.文件IO与标准IO的区比

	区别:响应速度 & 吞吐量
	面试:如何使一个程序变快?
		标准IO吞吐量大,有缓冲区,fflush
		文件IO响应速度快,没有缓冲区,直接刷新
			对用户来说,速度快一般是吞吐量大
	提醒:标准IO和文件IO不可以混用
	转换:fileno,fdopen,在文件IO和标准IO相互转换
	标准IO拿到FILE,文件IO拿到fd

4.IO的效率问题
习题:将mycpy.c程序进行更改,把BUFFSIZE的值进行放大,并观察进程消耗的时间,注意性能最佳拐点的BUFFSIZE值
	以及何时程序会出现问题(和stack size有关 ulimit -a)(段错误)

	strace ./ab
	可以看到实际执行了啥	

	./mycpy /etc/services /tmp/out
	time ./mycpy /etc/services /tmp/out
	real	0m0.002s
	user	0m0.001s
	sys	0m0.000s;


5.文件共享:多个任务共同操作一个文件或者协同完成一个任务
面试: 写程序删除一个文件的第10行.
	1).
		while()
		{
			lseek 11 +read +lseek10 +write
		}
	2).
		1 ->open r -> fd1 ->lseek 11
		2 ->open r+-> fd2 ->lseek 10
		while()
		{
			1-> fd1 -> read
			2-> fd2 -> write
		}
	3)两个进程间通信,多线程	
		process1 -> open ->r
		process2 -> open ->r+
	
		thread1 
		thread2
补充函数:truncate/ftruncate截断文件长度

6.原子操作:不可分割的操作
	原子:不可分割的最小单位
	原子操作的作用:解决竞争和冲突
	如tmpnam

7.程序中的重定向：dup，dup2


8.同步：sync,fsync,fdatasync

9.fcntl();管家级的函数
	fcntl():文件描述符所变的魔术几乎都来源于该函数

10.ioctl();管家级的函数
	ioctl():设备相关的内容
	设备开发会涉及它,是一切皆文件的垃圾堆

11.虚目录：/dev/fd/目录
	显示的是当前文件描述符信息


/****************************************************************************/

				文件系统
类ls的实现,如myls -l -a -i -n

知识性的内容

一. 目录和文件
	1.获取文件属性
		int stat(const char *pathname, struct stat *statbuf);//面对符号链接文件获取的是所指向的目标文件的属性
		int fstat(int fd, struct stat *statbuf);
		int lstat(const char *pathname, struct stat *statbuf);//面对符号链接文件时获取的是符号链接文件的属性

	struct stat {
		dev_t     st_dev;         /* ID of device containing file */
		ino_t     st_ino;         /* Inode number */
		mode_t    st_mode;        /* File type and mode */
		nlink_t   st_nlink;       /* Number of hard links */
		uid_t     st_uid;         /* User ID of owner */
		gid_t     st_gid;         /* Group ID of owner */
		dev_t     st_rdev;        /* Device ID (if special file) */
		off_t     st_size;        /* Total size, in bytes */
	
		blksize_t st_blksize;     /* Block size for filesystem I/O */
		blkcnt_t  st_blocks;      /* Number of 512B blocks allocated */

		/* Since Linux 2.6, the kernel supports nanosecond
		   precision for the following timestamp fields.
		   For the details before Linux 2.6, see NOTES. */

		struct timespec st_atim;  /* Time of last access */
		struct timespec st_mtim;  /* Time of last modification */
		struct timespec st_ctim;  /* Time of last status change */

#define st_atime st_atim.tv_sec      /* Backward compatibility */
#define st_mtime st_mtim.tv_sec
#define st_ctime st_ctim.tv_sec
	};
注意:	st_size不等于st_blksize * st_blicks 
wangwenghan@wangwenghan-linuxlearn:~/Documents/linux_c/io/fs$ stat flen.c
  File: flen.c
  Size: 414       	Blocks: 8          IO Block: 4096   regular file
  Device: 803h/2051d	Inode: 1601226     Links: 1
blocks = 8 ,8 *0.5k = 4k, 4k硬盘大小的应用大小为414
	
	unix下size只是一个属性,和硬盘占用量不一样,和windows不一样


	2.文件访问权限
		st_mode是一个16位的位图,用于表示文件类型,文件访问权限,及特殊权限位		
		struct stat statres;

		S_ISSOCK(statres.st_mode)
		S_ISREG(statres.st_mode)
	3.umask
		作用:防止产生权限过松的文件	

	4.文件权限的更改/管理chmod,fchmod
		chmod
		fchmod
	5.粘住位
		t位

	6.文件系统:FAT,UFS  同一时期产物,FAT windows 闭源, UFS unix开源
		文件系统:文件或数据的存储和管理
		FAT16/32:静态单链表  存储能力有限,现在u盘在用  p146
		UFS:		p147		


	7.硬链接,符号链接
		ln 		硬链接  源文件删除,链接文件可用   links+1
			size与源文件大小一致
			相当于两个指针指向同一数据块
		ln -s(symbol) 符号链接  源文件删除,链接文件不可用 links不变
			size为源文件文件名的长度的大小
			符号链接文件并不占磁盘大小,而是放到inode节点中				
		lrwxrwxrwx. 的l表示符号链接文件

		硬链接与目录项是同义词,且建立硬链接有限制:不能给分区建立,不能给目录建立 . 符号链接优点:可跨分区,可以给目录建立	

		link()
		unlink() 
		remove()	
		rename()
		
	8.目录utime:可以更改文件的最后读的时间和最后修改的时间
		

	9.目录的创建和销毁
		mkdir
		rmdir

	10.更改当前工作路径
		chdir
		fchdir
		getcwd
		能够突破假根技术,所以被chroot限制住的用户要有权限限制

	11.更改目录/读取目录内容
			第一种方法:glob解析目录
		glob():解析模式/通配符
			
			第二种方式:把目录当作流来使用	
		DIR *opendir(const char *name);
		int closedir(DIR *dirp);
		struct dirent *readdir(DIR *dirp);
	struct dirent {
               ino_t          d_ino;       /* Inode number */
               off_t          d_off;       /* Not an offset; see below */
               unsigned short d_reclen;    /* Length of this record */
               unsigned char  d_type;      /* Type of file; not supported
                                              by all filesystem types */
               char           d_name[256]; /* Null-terminated filename */
           };
	有了name,就能用stat()获取文件属性了
		
作业:使用这几个函数把mydu.c重构

		rewinddir():
		seekdir():
		telldir():

二. 系统数据文件和信息


1 /etc/passwd
	FreeBSD -> BDB 一种非关系型数据库来存储
	HP UNIX -> fs  文件系统来存,一个目录放一个26个子目录去找
	Linux -> /etc/passwd

	struct passwd *getpwnam(const char *name);
	struct passwd *getpwuid(uid_t uid);

struct passwd {
               char   *pw_name;       /* username */
               char   *pw_passwd;     /* user password */
               uid_t   pw_uid;        /* user ID */
               gid_t   pw_gid;        /* group ID */
               char   *pw_gecos;      /* user information */
               char   *pw_dir;        /* home directory */
               char   *pw_shell;      /* shell program */
           };



2 /etc/group
	struct group *getgrnam(const char *name);
	struct group *getgrgid(gid_t gid);

 struct group {
               char   *gr_name;        /* group name */
               char   *gr_passwd;      /* group password */
               gid_t   gr_gid;         /* group ID */
               char  **gr_mem;         /* NULL-terminated array of pointers
                                          to names of group members */
           };


3 /etc/shadow
	//通过用户名获取用户的struct spwd
	struct spwd *getspnam(const char *name);
	struct spwd *getspent(void);
struct spwd {
               char *sp_namp;     /* Login name */
		

               char *sp_pwdp;     /* Encrypted password */ 
               long  sp_lstchg;   /* Date of last change
                                     (measured in days since
                                     1970-01-01 00:00:00 +0000 (UTC)) */
               long  sp_min;      /* Min # of days between changes */
               long  sp_max;      /* Max # of days between changes */
               long  sp_warn;     /* # of days before password expires
                                     to warn user to change it */
               long  sp_inact;    /* # of days after password expires
                                     until account is disabled */
               long  sp_expire;   /* Date when account expires
                                     (measured in days since
                                     1970-01-01 00:00:00 +0000 (UTC)) */
               unsigned long sp_flag;  /* Reserved */
           };
	
	//获取的是/etc/shadow下的第三个$之前的内容,之后的crypt看不到
	shadowline->sp_pwdp
	//密码是phrase,加密的setting是setting
	char * crypt(const char *phrase, const char *setting);
	//实现关闭回显获取字符串
	char *getpass(const char *prompt);




4 时间戳:time_t     char*       struct tm
		书上p153
	time();
		time_t stamp;

		time(&stamp);
		stamp = time(NULL);

	gmtime();
		
	localtime();
//mktime会判断tm是否合法,不合法会进行调整
	mktime();
	strftime();
		tm = localtime(&stamp);
		strftime(buf,BUFFSIZE,"%Y:%m-%d",tm);
		puts(buf);	

三. 进程环境
1.main函数
	int main(int argc,char**argv)

2.进程的终止   *****
	正常终止:
		从main函数返回
		调用exit
		调用_exit或_EXIT
		最后一个线程从其启动例程返回
		最后一个线程调用了pthread_exit

	异常终止:
		调用abort
		接到一个信号并终止
		最后一个线程对其取消请求做出响应

	atexit();钩子函数    *****
	
3.命令行参数的分析   *****
	mydate.c
	getopt();
	//如果读到非选项传参,那么返回值为1,全局变量optind定位到读的位置,读完之后已经指向下一个argv,所以argv[optind-1]为刚刚读过的非选项参数
	getopt_long();

4.环境变量    *****
	export
	KEY = VALUE

	全局变量:extern char ** environ;

	getenv();
	//环境变量改变的话是释放掉原本的空间再去申请新的堆空间把值放上去
	setenv();
	//没有const修饰,不好用,会改值
	putenv();
	

5.C程序的存储空间布局      *****
	ps axf;查看进程id
	pmap(1);


6.库
	动态库
	静态库
	手工装载库(内核中模块的本质就是插件)这些就是手工装载库
	看example的手动装载库
	dlopen();
	dlclose();
	dlerror();
	dlsym();
	

7.函数跳转	setjmp.c  *****

	setjmp();
	longjmp();


8.资源的获取及控制
	ulimit -a
	getrlimit();
	setrlimit();
	




/****************************************************************************/

				进程基本知识
				进程间通信:IPC
已经进入多进程阶段

1. 进程标识符 pid
	类型pid_t 传统上有符号的16位整形数,每个机器上不确定
	命令 ps
	进程号是顺次向下使用
	getpid();
	getppid();
	
2. 进程的产生		fork*.c
	fork();
	注意理解关键字:duplicating,意味着拷贝,克隆,一模一样等含义
	fork后父子进程的区别: fork的返回值不一样, pid不同,ppid也不同,未决信号和文件锁不继承,资源利用率清0
	
	init进程:init出来前内核是一个程序在跑,产生之后内核变成一个库守在后台,每次出现异常时跳出来解决异常
	init进程:1号,是所有进程的祖先进程	
	
	调度器的调度策略来决定哪个进程先运行

	fflush();的重要性://fork前一定要刷新缓冲区,否则子进程的缓冲区也有一句begin
	fork()加了一个写时拷贝的技术,谁改谁拷贝到新的,

	fork里的子进程执行的代码一定要有exit,否则子进程也会继续创建子进程

	vfork();//基本废弃了,子进程会立即调用exec或者exit,其他行为均未定义


3. 进程的消亡及释放资源
	zombie状态的进程不占进程,只是一个结构体,包含pid,退出状态等.由于pid很宝贵,所以需要收尸
	wait(); 死等
	waitpid();  
	waitid();
	
	方言:
	wait3();
	wait4();

	分配进程的方法
	1 . 分配法
	2 . 交叉分配  primerN.c
	3 . 池 (如何让其它进程知道你抢到了这个数涉及到进程间通信)

4. exec函数族		few.c
	exec前也一定要fflush(NULL);
	fork前也一定要ffush(NULL);
	
	仅仅fork的话子进程和父进程一样,所以要exec把子进程改变成其他函数
	仅仅将此程序改变运行内容,pid没变,所以一般用fork出的子进程运行exec函数,父进程wait(NULL)等待返回,这样使父子进程可以不一样了
	execl();
	execlp();
	execle();
	execv();
	execvp();
	execvpe();

	shell -> fork -> 子进程 -> 等待子进程结束 -> wait收尸
	所以ls等命令是先结束后打印出信息,shell收尸wait后打印命令行

5. 用户权限及组权限(u+s,g+s)		mysu.c
	u+s
	g+s
	uid 和 gid 不止一份, 一共三份,real id(真实的), effective id(有效的,鉴定权限时用的effective), save id(可以没有)
	shell fork+exec产生了子进程passwd,passwd继承父进程的r e s权限,exec发起了u+s的权限,
		exec来鉴定权限,发现是u+s,那么子程序passwd的身份r不变,e和s改变为0,看着e的权限跑程序,跑完并没有把身份切换回去,但是passwd程序消亡,shell收尸后打印继续等待.
	例如 passwd实现普通用户该passwd,即/etc/shadow的修改权
	u+s,当别的用户在调用当前可执行文件时,身份会切换当前文件的user
	g+s,调用当前可执行文件,身份会切换为当前文件的同组用户group的权限
	
	init通过fork和exec产生getey,输入账号,之后并没有fork,而是exec,摇身一变变为login进程,产生getey和login进程身份都是root身份即0号的身份,然后用口令原文+uername找到/etc/shadow的加密那行进行加密,如果结果和所保存的一致,说明登陆成功.成功后,fork+exec产生shell,此时带着身份下来的,即user的身份

	getuid() returns the real user ID of the calling process.
	geteuid() returns the effective user ID of the calling process.
	getgid();
	getegid();

	setuid()  sets  the  effective  user ID of the calling process. 
	setgid();
	setreuid();交换ruid和euid,原子化的交换
	setregid();交换rgid和egid,原子化的交换

	seteuid(); sets the effective user ID of the calling process.
	setegid();


6. 观摩课: 解释器文件

7. system();		system1.c 和 few.c 比较		system.c
	man system 里规定的用法，/bin/sh -c 后面接command
	理解:fork + exec + wait的封装
	
8. 进程会计
	统计当前进程

	acct(); //方言SVr4, 4.3BSD (but not POSIX). 不可移植
	
9. 进程时间
	times();


10. 守护进程		p179		mydaemon.c
	一般脱离控制终端,一般是一个会话的leader,一个group process的leader.
	
	会话session,标识sid
		一个session,可以有多个进程组,一个进程有一个或一个以上的线程.进程组要分成前台进程组和后台进程组,前台进程组最多有一个.前台能够使用标准输入输出,将标准输入内容给一个后台正在运行的进程会将这个后台进程杀死.
	终端
	
	setsid();
	getpgrp();
	getpgid();
	setpgid();

	单实例的守护进程 : 锁文件 /var/run/name.pid
	每次开机启动,守护进程会在/var/run/下生成锁文件,把当前的进程编号写进去,当再次start时,发现锁文件已经有了该进程运行就不会再启动了
	
	开机启动脚本文件:  /etc/rc*...

11. 系统日志		/var/log
	每个应用程序都要去写系统日志
	syslogd服务
	void openlog(const char *ident, int option, int facility);//人物,特殊要求,消息来源         无返回值,不会出错
	void syslog(int priority, const char *format, ...);//级别(以error和waring为分界点),格式
	void closelog(void); 



/*************************************************************************/

			并发(信号 (初步异步), 线程(强烈异步) )
同步
异步

异步事件的处理 : 查询法 , 通知法

单核叫并发,多核叫并行
并发是指一个处理器同时处理多个任务。 并行是指多个处理器或者是多核的处理器同时处理多个不同的任务。

一  信号

1. 信号的概念		kill -l
	信号是软件层面的中断.
	信号的响应依赖于中断.

	信号有默认动作,最多的有 终止+core .

	https://blog.csdn.net/weixin_43729127/article/details/131856080
	程序出错产生core文件,可以用gdb core文件来调试程序

2. 信号的发出 signal();多个信号来共用同一信号函数会出现重入
	typedef void (*sighandler_t)(int);
		+
       	sighandler_t signal(int signum, sighandler_t handler);
		=
	void (*signal(int signum, void (*func)(int))) (int);

	ctrl+c 是SIGINT的快捷方式
	ctrl+\ 是SIGQUIT的快捷方式

	

	信号会打断阻塞的系统调用


3. 信号的不可靠(指的是行为不可靠)
	标准信号一定要丢失,实时信号不丢失
	此处指的是信号的行为不可靠指的是行为不可靠
	
	信号没有人为的去写调用语句,只是规定信号到来时去做这个指定的动作,调用的时候执行现场是由内核布置的,信号的行为不可靠是指一个信号在处理一个行为的同时又来了一个相同的信号,由于执行现场是由内核布置的,很有可能不在同一位置,那么第二次的执行现场就把第一次的冲掉了


4. 可重入函数
	第一次调用没有结束就发生第二次调用但是并没有出错
	
	所有的系统调用都是可重入的,一部分库函数也是可重入的,如:memcpy



//看完操作系统再看一遍
5. 信号的响应过程(老师画的图很重要)    p185
	内核为每个进程维护了最少两个位图,一个mask,一个pending,标准信号的大小,所以32位
	mask:信号屏蔽字,表示当前信号状态,一般全部都是1
	pending:当前进程收到哪些信号,初始值为0

	信号从收到到响应有一个不可避免的延迟
中断把当前执行过程打断,扎内核即将往用户态走的时候才能看到那个信号
	思考:如何忽略掉一个信号的? mask这个位设置为0
	     标准信号为什么要丢失?	在上次响应过程中,假如一次来了一万个,只能将其置为1,只能响应一次
		标准信号的响应没有严格的顺序.
		不能从信号处理函数中任意往外跳 setjmp和longjmp,将会错过m复位的过程,(sigsetjmp,siglongjmp)
		位图,标准信号来一万次1还是1

标准信号响应过程:

正常发一次信号:
	M  P
	1  0	//刚开始
	1  1	//受到信号,变成1 1.而收到中断后扎内核,排队,在往user态走的时侯m&p得到1,表示受到信号
	0  0	//响应信号,均置为0,响应
	1  0	//响应完,m置为1,在做一次&,发现没有信号,就走了

一直发一个信号,好比一次发了10w个同一个信号
	M  P
	1  0
	1  1
	0  0	//响应信号
	0  1	//在响应信号时又来了1w个信号,p置为1
	1  1	//响应结束后m置为1,再做一次&,发现又来了个信号,继续响应,只响应一次
		


		
	
6.  信号常用函数
	kill();  //自己给自己发送信号

	raise(); //自己给自己发
	alarm(); //时间相关,一个程序只能有一个alarm,否则出错,按最后一个算
	setitimer(); //原子化的操作,并且误差不会累积,建议将alarm改为setitimer
例:使用单一计时器,利用alarm或者setitimer构造一组函数,实现任意数量的计时器
	

	pause(); //等待一个信号,阻塞调用,可以停止while的cpu沾满

	abort();//发送SIGABRT信号,目的结束当前进程顺便生成一个coredump文件
	system();// During  execution  of  the command, SIGCHLD will be blocked, and SIGINT and SIGQUIT will be ignored,in the process  that  calls  system().
	sleep();//尽量不要用,有的环境下,sleep -> alarm + pause,如果这样那你代码中再出现alarm会导致时钟错乱. 当前linux环境下,sleep -> nanosleep,可以用,但为了移植性,代码中出现这句话,直接不要写了,转行吧

	usleep(),nanosleep()可以来替换sleep

	select():也可以实现休眠


7. 信号集
	信号集类型:sigset_t	
	sigemptyset();
	sigfillset();
	sigaddset();
	sigdelset();
	sigismember();

	
8. 信号屏蔽字和信号pending集的处理	block.c
	sigprocmask();
	sigpending();//收到的是响应前的sigset_t

9. sleep();
	


10. 扩展
	sigsuspend();	//可以做信号驱动程序	susp.c sigprocmask和pause的操作不原子,所以sigsuspend用这个相当于用新的sigset_t信号阻塞,收到信号后接受信号并改回原sigset_t,此操作是原子的,在解除阻塞时马上进入等待信号阶段

	sigaction();	//可替换前面的signal ,参数很多,可以选择只接受从内核传过来的信号,防止用户态主动发送信号导致错乱
	settitimer();	//alarm


11. 实时信号(前面的全是标准信号)
	标准信号特点	1.会丢失,
			2.响应没有严格的顺序,只有大概的层次
	实时信号特点	1.要排队的
			2.响应有顺序要求
	一个进程中即收到标准信号又收到实时信号,会优先执行标准信号

	kill -l
	//换位置了
	vim usr/include/bits/signum.h
	ulimit -a里有个pending signals 表示的实时信号可以排多少


二  线程

线程工作模式:
	1.流水线 * -> 0 -> 0 -> 0
	2.工作组模式	0 -> 0 -> 0
			  -> 0 ->
			  -> 0 ->
	3.c/s	客户端,服务器端



1. 线程的概念
	main线程,不要说主线程,没有主次之分
	多个线程内存共享,因为在同一地址空间,
	一般发布出来的库都支持多线程并发,如果不可多并发,要说明
	
	一个正在运行的函数
	posix线程是一套标准,而不是实现
	openmp线程  ,也是一套标准
	线程标准是一套标准,而不是实现
	线程标识 : pthread_t	p指的是posix,pthread_t具体类型不知道,线程标识只是有这个东西,具体怎么实现看各家实现
	函数:
	pthread_equal
	pthread_self

2. 线程的创建
	pthread_create;
	int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                          void *(*start_routine) (void *), void *arg);
	第一个参数是线程id的地址,第二个是状态,默认填NULL,第三个是线程函数,第四个是线程所需参数
	线程的调度取决于调度器策略

   线程的终止
	3种方式:
	1) 线程从启动例程返回,返回值就是线程的退出码
	2) 线程可以被同一线程中的其他线程取消
	3) 线程调用pthread_exit()函数

	return一般表示的是函数的退出,线程的退出要用pthread_exit
	线程的清理需要用pthread_exit

	pthread_join();线程的收尸 --->  wait();
	int pthread_join(pthread_t thread, void **retval);
	retval传一级指针的地址用于查看状态,查看收尸的状态	
	pthread_join可指定要收的线程,wait是收一个尸,收回来才知道是谁

   栈的清理
	pthread_cleanup_push();
		
	pthread_cleanup_pop();
	这两个是宏,不是函数
	gcc cleanup.c -E
	pthread_clean_pop如果放到pthread_exit后仍然会被调用,不过默认为1
		参数决定钩子函数是否被调用,真,说明从钩子上取出来的函数需要被调用,假,不被调用
	


   线程的取消选项
	cancel后才可以收尸pthread_join
	线程取消:
		pthread_cancel();
		取消有两种状态: 允许 和 不允许
		允许取消又分为 : 异步cancel  和  推迟cancel(默认) ->推迟到cancel点再去响应
		cancel点: POSIX定义的cancel点,都是可能引发阻塞的系统调用

		int pthread_setcancelstate(int state, int *oldstate);设置是否允许取消
		int pthread_setcanceltype(int type, int *oldtype);设置取消方式(异步cancel或者推迟cancel)

		void pthread_testcancel(void);设置取消点,本函数什么都不做,就是一个取消点.如果函数中全是科学计算,没有任何的系统调用,所以没有cancel点,此时发现出错要取消,需要设置cancel点才能取消
	
    线程分离:
		int pthread_detach(pthread_t thread);不关心这个线程的生死存亡,无需回收,设置了线程分离就不能pthread_join回来


3. 线程同步	abcd.c	abcd_e.c  mytbf
	互斥量:	pthread_mutex_t
		pthread_mutex_init()
		pthread_mutex_destroy();
		
		pthread_mutex_lock();
		pthread_mutex_trylock();
		pthread_mutex_unlock();

		pthread_once();只调用一次
		pthread_once(&init_once,module_load);
		相当于
		/*
		   pthread_mutex_lock();
		   if(!inited)
		   {
		   module_load();
		   inited = 1;
		   }
		   unlock();
		 */
	
	条件变量(通知法):	mytbf.c
		pthread_cond_t cond;
		pthread_cond_init
		pthread_cond_destroy
		pthread_cond_broadcast(pthread_cont_t
*cond);//发消息,发通知打断一个pthread_cond_wait,全部叫醒
		pthread_cond_signal();//叫醒某一个不清楚
		pthread_cond_timewait();
		pthread_cond_wait();,没有锁就先等,等到抢到锁
	
	信号量: 没有一个单独的机制
		互斥量+条件变量实现一个 	mysem.c
		mysem
		10人吃面,5根筷子,发信号谁用谁拿

	读写锁:
		读锁->共享锁 + 写锁->互斥锁
	r -> 共享
	w -> 互斥
	避免写者饿死:
两个人读文件,加了读锁,来了一个写文件,w等待,但是其他读者不再加读锁,相当于加了个w锁,等两个人读完,w进去,等w退出,释放w锁,其他可再读.
	


	临界区要注意锁的情况,要注意临界区内所有的跳转语句:continue,break,函数,goto,如果跳转到临界区外,一定要先解锁再跳转
primer0_busy.c


4. 线程属性		create2.c
	pthread_create()的第二个参数const pthread_attr_t *attr就是线程属性
	
	pthread_attr_init(pthread_attr_t *attr);
	pthread_attr_setstacksize();
	见man手册pthread_attr_init的see also

	当前64位环境,一个线程占1024*1024*1024大小的stack,可以创建4553个,每个线程创建一个i,取i的地址
0x7fccf3bfde44
0x7fcc6bbfbe44
0x7fcd33bfee44
0x7fcc2bbfae44
0x7fcbebbf9e44
0x7fcbabbf8e44
0x7fccb3bfce44
0x7fcb6bbf7e44
count = 4553

   线程同步的属性
	互斥量属性: 
		pthread_mutexattr_init();
		pthread_mutexattr_destroy();
		pthread_mutex_getpshared();
		pthread_mutex_setpshared();//p代表process,是否跨进程
		为什么线程还要跨进程,涉及函数clone
		clone();
//多线程共享了一些东西,多进程共享了一些东西
//如果clone的flags用属性的极端分离,即什么都用自己的,那么创建的就是之前说的进程
//如果什么都共用,文件描述符表共用,父进程也是同一个,文件系统信息也是同一个等等,那么创建的就是兄弟,相当于ptrhead_create的动作,创建了一个线程
如果现实中发现多线程和多进程都不好用,一些需要共用,另一些不要共用,那么就可以用这个函数clone,其实进程线程就没有分的那么清楚
		 CLONE_FILES (since Linux 2.0)
              If CLONE_FILES is set, the calling process and the child process
              share  the same file descriptor table.
		fork是复制文件描述表,cone中的flags设置为CLONE_FILES表示共享文件描述表,实现进程间通信	
		 CLONE_NEWPID (since Linux 2.6.24)
		CLONE_PARENT(since Linux 2.3.12)

		pthread_mutexattr_gettype();
		pthread_mutexattr_settype();

	条件变量属性:	
		pthread_condattr_init()
		pthread_condattr_destroy();
	读写锁属性:
		


5. 重入:   信号产生重入,信号更难;线程重入,更简单
	线程重入
		1->puts(aaaaaa)
		2->puts(bbbbbb)
		3->puts(cccccc)
			结果:aaaaaabbbbbbbbccccc都可能
			不会出现abbbbaaccc交替输出的现象,因为多线程中的IO,库函数支持重入
	
	未加锁的函数:getchar_unlock();
	多线程中的IO就是先把流锁住再解锁.

	
   线程与信号的关系
	每条线程都会有自己的mask和pending. 
	如果以进程为单位,没有mask,只有pending
	如果一个线程给另一个线程发信号,那么实现的是在线程的pening的某一位上.
	响应进程范围内的信号,要看当前从kernel态扎回到用户态调度的是那个线程,扎回来的时候做一个按位与,拿线程自己的mask和进程的pending做一个按位与,看一下以进程为单位收到了那些信号,再用线程自己的mask和线程自己的pending做按位与,再看当前线程收到了那些通知或者信号,所以其实是作了两次,之前说的模型是单进程单线程,线程级别的pending并未提及

	
	pthread_sigmask(); 相当于进程阶段的sigprocmask();
	sigwait();
	pthread_kill(); 类似kill();

   线程与fork
	posix线程中中fork:新的进程中只包含调用fork的那一个线程
	dce线程认为,执行fork时就和原来进程是一样的
	
	文件锁可能会在创建子进程后也会出现意外. 文件锁在close时也会出现意外
	posix只提供了类型推荐,标准,具体实现自己去实现
	
	openmp线程  ->  www.OpenMP.org
	


创建线程和创建进程效率差几倍,通信方式上进程有什么特点,线程有什么特点



/***************************************************************************/

				高级IO
非阻塞IO ------ 阻塞IO
阻塞和非阻塞的两种假错: EINTR(blocking,必须做到为止,出现打断信号就返回EINTR)和EAGAIN(non-blocking,尝试去读,没有就返回)

补充:有限状态机编程

1. 非阻塞IO
	数据中继原理解析	p220   数据中继原理.png
	简单流程 : 自然流程是结构化的
	复杂流程 : 自然流程不是结构化的
		
	中继引擎实例 : 	nonblock/relayer.c 未完成,Aborted (core dumped)
		IO密集型任务:IO多 负载重任务:大部分时间都在空闲



2. IO多路转接(文件描述符的监视)
	select();以事件为单位来组织文件描述符	古老,可移植
		adv/select# ./relay 

	poll(); 以文件描述符来组织事件		可移植  移植性和效率均可
		
	epoll();各自平台做的方言,不可移植,效率一般较高
	EPOLL(7) 第7章讲机制,说了当前平台下的epoll怎么实现

	sleep时提到了select,select前面的参数如果给的没有实际意义,而最后一个参数给了timeout,也可以实现sleep的功能
	
	int select(int nfds, fd_set *readfds, fd_set *writefds,
                  fd_set *exceptfds, struct timeval *timeout);
	nfds: 要监视的文件描述符的最大值+1,比如要监视1,3,4,8,那么这里要写9
	readfds:所关心的可读的文件描述符
	writfds:所关心的可写文件描述符
	exceptfds:所关心的异常的文件描述符
	timeout:-1,NULL,NULL,NULL,timeout不设置超时设置,就是死等,阻塞,直到等到感兴趣的事件发生
	return value:返回值的个数是现在发生了你感兴趣的行为的文件描述符的个数
		On success, select() and pselect() return the number of  file  descriptors  contained in the three returned descriptor sets (that is, the to‐
       tal number of bits that are set in readfds, writefds, exceptfds).   The
       return  value  may  be  zero if the timeout expired before any file de‐
       scriptors became ready.

       On error, -1 is returned, and errno is set to indicate the  error;  the
       file descriptor sets are unmodified, and timeout becomes undefined.

	
	select:缺陷:	1.首个是int类型
			2.没有const修饰
			3.监视现场和监视结果放同一块空间
			4.监视事件太过单一,读写异常
			事件为单位监视文件描述符
	poll: wait for some event on a file descriptor,以文件描述符组织事件
			0表示非阻塞,-1表示阻塞,合法数值以毫秒为单位1000为1s
		user	struct pollfd[n]    [0][1][2][3][4][5]用户态维护的数组
		-----------------------------------------------
		kernal
		从用户角度维护的数组
		
	epoll: 

		user
		---------------------------------------------
		kernal	内核  n	[][][][][][][]
		kernal为你维护了这个数组来帮助管理所有的文件描述符的关心的事件,返回的事件等等,epoll_create()就是在这实现的数组,不过这个size写几都可以,是让内核以多大的数组来维护文件描述符,常规数值即可
		

3. 其他读写函数

	多个碎片的小地址要写道同一个文件中
	read or write data into multiple buffers
	readv();
	writev();

	readn();
	wirten();坚持写够write个字节,就是之前的write会出现假错,apue的作者觉得要用就封装出来了
	


4. 存储映射IO 最好用的共享内存了
	存储映射IO能将一个磁盘文件映射到存储空间中的一个缓冲区上,于是,当从缓冲区取数据时,就相当于读文件中的相应字节.于此类似,将数据存入缓冲区时,相应字节就自动写入文件.这样,就可以在不使用read和write的情况下执行IO
	mmap();
	相当于把某一块内存,或者说某一块文件的存储内容映射到当前进程空间里,相当于访问一段char*的内容,就如同访问那段空间一样
	

	能帮我们作出一个非常快的共享内存
	void *mmap(void *addr, size_t length, int prot, int flags,
                  int fd, off_t offset);
		addr:把内存映射到地址空间的位置
	addr用NULL时就是自己找
	可以用mmap代替动态内存管理的函数malloc和free,如果flags用了MAP_ANONYOUS,匿名映射,相当于malloc
	从fd文件的offset偏移量开始,要length个字节放到addr地址来,映射过来的权限是prot,特殊要求是flags
	
	int munmap(void  *addr,size_t length);

	具有亲缘关系的进程间通信:	sharedmemory.c
	
	int munmap(void *addr, size_t length);



5. 文件锁
	fcntl();
	lockf();
	flock();
	
	文件锁.png
	通过文件描述符锁住一个文件,锁住的是inode上,而不是当前数组下标指向的文件信息结构体的层面,dup,dup2是把同一个文件信息结构体放到了数组的两个下标,同一个程序中对同一个文件打开两次,会生成两个文件信息结构体和两个fd,但是两个文件信息结构体指向同一个inode
	通过另一个fd来close同一个inode,会导致文件解锁

6.管道 thread/posix/mypipe/mypipe.c



/***************************************************************************/
				ipc
	
				进程间通信
			同一台机器通信,不同机器通信
1. 管道 
	管道实际上就是磁盘上一文件

	内核提供,单工,自同步机制(牵就慢的一方)
	//这两种管道都是内核为你创建的
	匿名管道	
		pipe()
	命名管道
		mkfifo()
		
2. XSI ipc --前身-> SysV	自己写通信方式就是封装协议了
	IPC -> Inter-Process Communication 进程间通信


Documents/linux_c/io/ipc/xsi/msg/basic

	主动端: 先发包的一方
	被动端: 先收包的一方(先运行)


	有亲缘关系的进程通信都可以,fork后子进程都能拿到,没有关系的进程关系就要用key
	key : ftok(const char* pathname,int proj_id);产生同一种key值
	pathname相当于哈希内容,proj_id相当于哈希杂质

	利用同一个key值产生message queues,semaphore arrays和share memory sements

	xxxget xxxop(使用)  xxxctl(控制) -> xxx -> msg sem shm

Message Queues
	https://www.cnblogs.com/52php/p/5862114.html
	
	msgget()
	
	msgop() //消息队列双工的
ssize_t msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp,
                      int msgflg);
msqid:msgget的返回值就是msgid.   msgp:要放到的空间   msgsz:要放的大小  msgtyp:
取当中第几个包,假如消息队列10个包,取第msgtyp个包,其他还在排
msgflg:特殊要求,位图
The msgp argument is a pointer to a caller-defined structure of the following general form:

           struct msgbuf {
               long mtype;       /* message type, must be > 0 */
               char mtext[1];    /* message data */
           };

int msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg);
msqid:msgid  msgp:要取的空间  msgsz要取的大小  msgflg:特殊要求
msg_sz 是msg_ptr指向的消息的长度，注意是消息的长度，而不是整个结构体的长度，也就是说msg_sz是不包括长整型消息类型成员变量的长度。

	msgctl()			
		msgctl(msgid,IPC_RMID,NULL);
		//销毁msgid



Semaphore Arrays信号量数组
			例子:两个信号a和b,线程1锁住a请求b,线程2锁住b请求a出现死锁,
		数组为1就相当于之前的mut了
		可以让整个信号量数组原子化lock,避免死锁

	int semget(key_t key, int nsems, int semflg);
	key:key值   nsems:semaphore array中信号个数  semflg:一个元素
	semid = semget(IPC_PRIVATE, 1 , 0600);  //IPC_PRIVATE
,互斥量的个数,权限


	int semctl(int semid, int semnum, int cmd, ...);
	semid : semid  semnum :对谁,semphore num的下标值  cmd:做什么 

	semctl(semid ,0,SETVAL, 1 ) ;  //初始化,对semid的下标为0的参数setval,设置初始值为1

	semctl(semid , 0 , IPC_RMID );	//销毁, 用IPC_RMID的话0表示销毁当前semid,就不是下标了


	int semop(int semid, struct sembuf *sops, size_t nsops);
		semid:senid  sops:struct sembuf类型的一个数据   nsops:数组中有几个元素
	
	
	while( semop(semid,&op,1) < 0 ) //从资源量减1,做op,资源总量
        {
                if(error != EINTR || error != EAGAIN)//真错,结束
                {
                        perror();
                        exit(1);
                }
		//假错,继续做
        }

	









Shared Memory Segments共享内存

	shmget(IPC_PRIVATE,  ,);        //key_t key,要设置的共享内存的size大小,shmflg特殊要求	

	shmop

	shmctl


key : ftok()
首先创建实例xxxget,有必要的话要用ftok获取一个key值,用key值创建一个ipc实例,然后用xxxop获取


3. 网络套接字 socket
	讨论: 跨主机的传输要注意的问题
	1. 字节序问题: 大端存储   ,   小段存储
		大端: 低地址处放高字节
		小端: 低地址处放低字节
			   0x0000 0005
			大 05 00 00 00	 
			小 00 00 00 05
		文件传输或者IO都是低地址处先出去
		
		因此
			区分主机字节序 和 网络字节序
		
		主机字节序 : host
		网络字节序 : network
		解决 :_ to _ _ : htons , htonl , ntohs , ntohl
			 htons: host to network 2个字节16位
			 htonl: host to network 4个字节32位

	2. 对齐 :   解决:不对齐
		struct
		{
			//char ch1;
			int i;
			float f ;
			//char ch2;
			char ch ;
		};
		字存储,双字存储,半字存储
		基本都向字存储靠拢
		所以该结构体默认12 ,ch1和ch2只选一个,如果有ch1就是16,如果有ch2就12
		32位int4个字节对齐,64位可能出现其他情况导致通过指针没法找
	
	3. 类型长度问题
		int 占多大 16位占2个字节 ,32位占4个
		char有无符号
		解决: int32_t ,uint32_t , int64_t ,int8_t ,uint8_t

	socket是什么:
		网络实现方式			流式套接字  报式套接字  其他
		中间层		int 文件描述符<-socket
		协议族				ipv4 s025 ips 

	int socket(int domain, int type, int protocol);
		domain :协议族    type:类型   protocal:某种协议
		AF_PACKET,网卡驱动层,嗅探器/抓包器就是用类似这个写的,网络层抓不到报头装的什么

		消息队列映射到今天就是SOCK_SEQPACKET ,有序的,可靠的,双方建立连接的,报式通信
		


	报式套接字:
		


	流式套接字:











