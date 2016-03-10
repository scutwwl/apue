#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define MAXLINE 4096

int main(void)
{
	char buf[MAXLINE];
	int status;
	pid_t pid;

	printf("%% ");
	while (fgets(buf, MAXLINE, stdin) != NULL)
	{
		if (buf[strlen(buf) - 1] == '\n')
			buf[strlen(buf) - 1] = 0;

		if ((pid = fork()) < 0)
		{
			printf("error in fork process.\n");
			exit(1);
		}
		else if (pid == 0)
		{
			if (execlp(buf, buf, (char *)0) < 0)
			{
				printf("error in execlp.\n");
				exit(2);
			}
		}

		if ((pid = waitpid(pid, &status, 0)) < 0)
		{
			printf("error in waitpid.\n");
			exit(3);
		}

		printf("%% ");
	}
	exit(0);
}
