/* 
 * 有全局变量g_flag
 * 在主线程启动线程1， 打印字符，将g_flag设为1
 * 在主线程启动线程2，打印字符，将g_flag设为2
 * 线程1要在线程2结束后才能结束
 * 当主线程判断g_flag1变2或者2变1时，退出
 */

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>

int g_flag = 0;

pthread_mutex_t glock = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t gcond = PTHREAD_COND_INITIALIZER;

void *thr_fn1(void *arg)
{
	printf("this is thread1.\n");
	pthread_mutex_lock(&glock);
	if (g_flag == 2)
		pthread_cond_signal(&gcond);
	g_flag = 1;
	pthread_mutex_unlock(&glock);
	pthread_join(*(pthread_t *)arg, NULL);
	printf("thread 1 leave.\n");
	pthread_exit((void *)1);
}

void *thr_fn2(void *arg)
{
	printf("this is thread2.\n");
	pthread_mutex_lock(&glock);
	if (g_flag == 1)
		pthread_cond_signal(&gcond);
	g_flag = 2;
	pthread_mutex_unlock(&glock);
	printf("thread 2 leave.\n");
	pthread_exit((void *)2);
}

int main(void)
{
	int err;
	pthread_t tid1, tid2;

	err = pthread_create(&tid2, NULL, thr_fn2, NULL);
	if (err != 0)
	{
		printf("%s : %s.\n", __func__, strerror(err));
		exit(err);
	}

	err = pthread_create(&tid1, NULL, thr_fn1, (void *)&tid2);
	if (err != 0)
	{
		printf("%s : %s.\n", __func__, strerror(err));
		exit(err);
	}

	pthread_cond_wait(&gcond, &glock);

	sleep(1);
	printf("main function, %s\n", __func__);
	exit(0);
}
