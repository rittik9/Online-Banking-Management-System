//Rittik Panda
//MT2022090


#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include "string.h"
#include "./structures/account.h"

account acc1;

int main(int argc, char const *argv[])
{

	printf("<----------- WELCOME TO XYZ BANK -------------->\n"); 
	
	acc1.acc_no = 11111;// Admin's account no.
	
	printf("Hello Admin Please enter your password to begin :\n");
	scanf(" %[^\n]",acc1.password); 

	acc1.usertype=1; //making usertype of ADMIN as 1
	
	int fd = open("account", O_CREAT|O_TRUNC|O_RDWR, 0644);
	
	write(fd, &acc1, sizeof(acc1)); 

	close(fd);
	
	return 0;
}
