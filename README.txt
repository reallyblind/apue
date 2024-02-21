
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

	selete():也可以实现休眠


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

   线程的终止
   栈的清理
   线程的取消选项

3. 线程同步

4. 线程属性
   线程同步的属性

5. 重入
   线程与信号的关系
   线程与fork












