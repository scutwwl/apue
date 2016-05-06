/* program 9-12 */

#include "apue.h"
#include <errno.h>

static void sig_hup(int signo)
{
	printf("deal with the SIGHUP.\n");
	fflush(stdout);
}

static void pr_ids(char *name)
{
	printf("%s .\n", name);
}

int main(void)
{
	char c;
	pid_t pid;
	pr_ids("parent");
	if ((pid = fork()) < 0)
		err_sys("fork error");
	else if (pid > 0)
		sleep(5);
	else
	{
		pr_ids("child");
		signal(SIGHUP, sig_hup);
		kill(getpid(), SIGTSTP);
		pr_ids("child");
		if (read(STDIN_FILENO, &c, 1) != 1)
			printf("read error.\n");
	}

	exit(0);
}
