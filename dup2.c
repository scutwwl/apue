#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int mydup2(int oldfd, int newfd)
{
	int create_fd;
	int create_fd_arr[newfd];
	if (oldfd == newfd)
		return oldfd;

	close(newfd);

	while ((create_fd = dup(oldfd)) != newfd)
		create_fd_arr[create_fd] = 1;

	for (int i = 0; i < newfd; ++i)
		if (create_fd_arr[i] == 1)
			close(i);

	return newfd;
}
