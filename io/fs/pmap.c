#include<stdio.h>
#include<stdlib.h>

int quanju = 5;


int main()
{
	static int a = 0;
	int b = 1;
	double dou = 3.0;
	float flo = 2.0f;
	int init = 4;
	int * ptra_static = &a;
	int * ptr_int = &init;
	struct st
	{
		int st_int;
		double st_dou;
	};
	struct st s;
	struct st *ptr_s;
	ptr_s = malloc(sizeof(*ptr_s));
	getchar();
	exit(0);


}

