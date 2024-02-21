#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<shadow.h>
#include<crypt.h>
int main(int argc,char **argv)
{
	char *input_pass;
	struct spwd *shadowline;
	char * crypted_pass ;
	if(argc < 2)
	{
		fprintf(stderr,"Usage...\n");
		exit(1);
	}

	input_pass = getpass("password:");

	shadowline = getspnam(argv[1]);
	printf("%s\n",shadowline->sp_pwdp);
	crypted_pass = crypt(input_pass,shadowline->sp_pwdp);
	
	if(strcmp(shadowline->sp_pwdp,crypted_pass) == 0)
		puts("OK");
	else
		puts("false");


	exit(0);
}

