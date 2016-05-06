#include <sys/socket.h>

#define MAXSLEEP 128

int connect_retry(int sockfd, const struct sockaddr *addr, socklen_t alen)
{
	int numsec;
	/*使用指数补偿算法，当连接请求失败就会休眠，每次循环休眠时间以指数级增加*/
	for (numsec = 1; numsec <= MAXSLEEP; numsec <<= 1)
	{
		if (connect(sockfd, addr, alen) == 0)  //成功
		{
			//connection accepted and then return
			return 0;
		}
		//delay and sleep before trying again
		if (numsec <= MAXSLEEP/2)
			sleep(numsec);
	}
	return -1;  //当来到这里，说明即使retry，connect也不成功
}

int connect_retry_tran(int domain, int type, int protocol, const struct sockaddr *addr, socklen_t alen)
{
	int numsec, fd;
	for (numsec = 1; numsec <= MAXSLEEP; numsec <<= 1)
	{
		if (fd = socket(domain, type, protocol)) < 0)
			return -1;
		if (connect(fd, addr, alen) == 0)
		{
			//success and connection accepted
			return fd;
		}
		//connection not success
		close(fd);
		if (numsec <= MAXSLEEP/2)
			sleep(numsec);
	}
	return -1;
}


