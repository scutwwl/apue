#include <netdb.h>
#include <sys/socket.h>
#include <errno.h>
#include <syslog.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

#define BUFLEN 128
#define QLEN 10

#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 256
#endif

extern int initserver(int sockfd, const struct sockaddr *addr, socklen_t alen, int qlen);

int set_cloexec(int fd)
{
	int val;
	if ((val = fcntl(fd, F_GETFD, 0)) < 0)
		return -1;
	val |= FD_CLOEXEC;
	return (fcntl(fd, F_SETFD, val));
}

void serve(int sockfd)
{
	int clfd;
	FILE *fd;
	char buf[BUFLEN];

	set_cloexec(sockfd);
	for( ; ; )
	{
		if ((clfd = accept(sockfd, NULL, NULL)) < 0)
		{
			syslog(LOG_ERR, "ruptimed: accept error: %s", strerror(errno));
			exit(1);
		}
		set_cloexec(clfd);
		if ((fp = popen("/usr/bin/uptime", "r")) == NULL)          // 这里通过管道来实施子进程
		{
			sprintf(buf, "error:%s\n", strerror(errno));
			send(clfd, buf, strlen(buf), 0);
		}
		else
		{
			while (fgets(buf, BUFLEN, fp) != NULL)
				send(clfd, buf, strlen(buf), 0);
			pclose(fp);
		}
		close(clfd);
	}
}

int main(int argc, char *argv[])
{
	struct addrinfo *ailist, *aip;
	struct addrinfo hint;
	int sockfd, err, n;
	char *host;

	if (argc != 1)
	{
		printf("error in argc\n");
		exit(-1);
	}
	if ((n = sysconf(_SC_HOST_NAME_MAX)) < 0)
		n = HOST_NAME_MAX;
	if (gethostname(host, n) < 0)
	{
		printf("error in gethostname.\n");
		exit(-1);
	}
	//daemonize("ruptimed");
	memset(&hint, 0, sizeof(hint));
	hint.ai_flags = AI_CANONNAME;
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_addr = NULL;
	hint.ai_next = NULL;
	if ((err = getaddrinfo(host, "ruptime", &hint, &ailist)) != 0)
	{
		syslog(LOG_ERR, "ruptimed: getaddrinfo error : %s", gai_strerror(err));
		exit(1);
	}

	for (aip = ailist; aip != NULL; aip = aip->ai_next)
	{
		if ((sockfd = initserver(SOCK_STREAM, aip->ai_addr, aip->ai_addrlen, QLEN)) >= 0)
		{
			serve(sockfd);
			exit(0);
		}
	}
	eixt(1);
}
