#include <errno.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <netdb.h>

#define BUFLEN 128

extern int connect_retry_tran(int domain, int type, int protocol, const struct sockaddr *addr, socklen_t alen);

void print_uptime(int sockfd)
{
	int n;
	char buf[BUFLEN];

	while ((n = recv(sockfd, buf, BUFLEN, 0) > 0))
		write(STDOUT_FILENO, buf, n);
	if (n < 0)
	{
		printf("recv error\n");
		return -1;
	}
}

int main (int argc, char *argv[])
{
	struct addrinfo *ailist, *aip;
	sturct addrinfo hint;
	int sockfd, err;

	if (argc != 2)
	{
		printf("argc != 2, error\n");
		return -1;
	}
	memset(&hint, 0, sizeof(hint));
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_canonname = NULL;
	hint.ai_addr = NULL;
	hint.ai_next = NULL;
	if ((err = getaddrinfo(argv[1], "uptime", &hint, &ailist)) != 0)
	{
		printf("error in getaddrinfo\n");
		return -1;
	}
	for (aip = ailist; aip != NULL; aip = aip->ai_next)
	{
		if ((sockfd = connect_retry_tran(aip->ai_family, SOCK_STREAM, 0, aip->ai_addr, aip->ai_addrlen)) < 0)
			err = errno;
		else
		{
			print_uptime(sockfd);
			return 0;
		}
	}
	printf("cannot connect to server.\n");
	return -1;
}
