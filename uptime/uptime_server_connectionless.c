#include <netdb.h>
#include <errno.h>
#include <syslog.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFLEN 128
#define MAXADDRLEN 256

#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 256
#endif

void serve(int sockfd)
{
	int n;
	socklen_t alen;
	FILE *fp;
	char buf[BUFLEN];
	char abuf[MAXADDRLEN];

	struct sockaddr *addr = (struct sockaddr *) abuf;

	set_cloexec(sockfd);
	for ( ; ;)
	{
		alen  = MAXADDRLEN;
		n = recvfrom(sockfd, buf, BUFLEN, 0, addr, &alen);
		fp = popen("/usr/bin/uptime", "r");
		fgets(buf, BUFLEN, fp);
		sendto(sockfd, buf, strlen(buf), addr, alen);
		pclose(fp);
	}
}

int main(int argc, char *argv[])
{
	struct addrinfo *aip, *ailist;
	struct addrinfo hint;
	int sockfd, err, n;
	char *host;

	n = HOST_NAME_MAX;
	host = malloc(n);
	gethostname(host, n);
	//daemonize("ruptimed");
	memset(&hint, 0, sizeof(hint));
	hint.ai_flags = AI_CANONNAME;
	hint.ai_socktype = SOCK_DGRAM;
	hint.ai_canonname = NULL;
	hint.ai_addr = NULL;
	hint.ai_next = NULL;

	getaddrinfo(host, "ruptime", &hint, &ailist);
	for (aip = ailist; aip != NULL; aip = aip->ai_next)
	{
		if ((sockfd = initserver(SOCK_DGRAM, aip->ai_addr, aip->ai_addrlen, 0)) >= 0)
		{
			serve(sockfd);
			exit(0);
		}
	}
	exit(-1);
}

