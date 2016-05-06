#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void *thr_fn1(void *arg)
{
	printf("thread 1 returning.\n");
	return ((void *)1);
}

void *thr_fn2(void *arg)
{
	printf("thread 2 exiting.\n");
	pthread_exit((void *)2);
}

int main(void)
{
	int err;
	pthread_t tid1, tid2;
	void *tret;  // 用来存储pthread_exit的退出参数（表示了退出的状态）， 就是上面的(void *)2

	err = pthread_create(&tid1, NULL, thr_fn1, NULL);
	if (err != 0)
	{
		printf("cannot create thread1. the err = %d.\n", err);
		exit(err);
	}

	err = pthread_create(&tid2, NULL, thr_fn2, NULL);
	if (err != 0)
	{
		printf("cannot create thread2. the err = %d.\n", err);
		exit(err);
	}

	err = pthread_join(tid1, &tret);
	if (err != 0)
	{
		printf("cannot pthread_join.\n");
		exit(err);
	}
	printf("thread 1 exit code %ld.\n", (long)tret);

	err = pthread_join(tid2, &tret);
	if (err != 0)
	{
		printf("cannot pthread_join2.\n");
		exit(err);
	}
	printf("thread 2 exit code %ld.\n", (long)tret);
	
	exit(0);
}
