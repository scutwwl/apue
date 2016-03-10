#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	printf("the process id is %ld.\n", (long)getpid());
	printf("the parent process id is %ld.\n", (long)getppid());
	exit(0);
}
