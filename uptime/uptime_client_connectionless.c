#include <netdb.h>
#include <errno.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFLEN 128
#define TIMEOUT 20

void *sigalarm(int signo)
{
}

void print_uptime(int sockfd, struct addrinfo *aip)
{
	int n;
	char buf[BUFLEN];

	buf[0] = 0;
	if (sendto(sockfd, buf, 1, 0, aip->ai_addr, aip->ai_addrlen) < 0)
	{
		printf("error in sendto\n");
		exit(-1);
	}
	alarm(TIMEOUT);
	if ((n = recvfrom(sockfd, buf, BUFLEN, 0, NULL, NULL)) < 0)
	{
		if (errno != EINTR)
			alarm(0);
		printf("recvfrom error.\n");
		exit(-1);
	}
	alarm(0);
	write(STDOUT_FILENO, buf, n);
}

int main(int argc, char *argv[])
{
	struct addrinfo *ailist, *aip;
	struct addrinfo hint;
	int sockfd, err;
	struct sigaction sa;

	sa.sa_handler = sigalarm;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGALARM, &sa, NULL);
	memeset(&hint, 0, sizeof(hint));
	hint.ai_socktype = SOCK_DGRAM;
	hint.ai_canonname = NULL;
	hint.ai_addr = NULL;
	hint.ai_next = NULL;
	getaddrinfo(argv[1], "ruptime", &hint, &ailist);
	for (aip = ailist; aip != NULL; aip = aip->next)
	{
		sockfd = socket(aip->ai_family, SOCK_DGRAM, 0);
		print_uptime(sockfd, aip);
		exit(0);
	}
	exit(-1);
}
