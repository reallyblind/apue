#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>


int main()
{
	putchar('a');
	write(1,"b",1);

	putchar('a');
	write(1,"b",1);

	putchar('a');
	write(1,"b",1);

	exit(0);
}

//strace ./ab
/*
	brk(NULL)                               = 0x563a47b99000
	brk(0x563a47bba000)                     = 0x563a47bba000
	write(1, "b", 1b)                        = 1
	write(1, "b", 1b)                        = 1
	write(1, "b", 1b)                        = 1
	write(1, "aaa", 3aaa)                      = 3
	exit_group(0)                           = ?

 *
 *
 *
 * */



