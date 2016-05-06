/*  program 8-8  */

#include "apue.h"
#include <sys/wait.h>

int main(void)
{
	pid_t pid;
	if ((pid = fork()) < 0)
		err_sys("fork error");
	else if (pid == 0)
	{
		if ((pid = fork()) < 0)
			err_sys("fork error");
		else if (pid > 0)
			exit(0);

		/* first child is exited, and second child is doing things. */
		sleep(2);
		printf("this is second child.\n");
		exit(0);
	}

	if (waitpid(pid, NULL, 0) != pid)  // wait for the first child
		err_sys("wait error");

	printf("the father exit.\n");
	exit(0);
}
