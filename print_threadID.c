#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>

pthread_t ntid;

void printids(const char *s)
{
	pid_t pid;  // this is process id
	pthread_t tid;  // thread id

	pid = getpid();
	tid = pthread_self();

	printf("%s pid %lu tid %lu (0x%lx) \n", s, (unsigned long)pid, (unsigned long)tid, (unsigned long)tid);
}

void *thr_fn(void *arg)
{
	printids("new thread: ");
	return ((void *)0);
}

int main(void)
{
	int err;
	err = pthread_create(&ntid, NULL, thr_fn, NULL);
	if (err != 0)
	{
		printf("cannot create thread. The err = %d.\n", err);
		exit(err);
	}

	printids("main thread: ");
	sleep(1);
	exit(0);
}
