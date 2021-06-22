#include "includes/webserver.h"

bool method_get(int *fd, int server, Socket &sock, char *request)
{
	(void)sock;
	if (*fd != server)
	{
		printf("[GET REQUEST] %s\n", request);
		fflush(stdout);
		return (1);
	}
	return (0);
}

bool method_post(int *fd, int server, struct poll* s_poll, struct sockaddr *addr, Socket &sock)
{
	(void)fd;
	(void)server;
	(void)s_poll;
	(void)addr;
	(void)sock;
	return (0);
}

bool method_delete(int *fd, int server, struct poll* s_poll, struct sockaddr *addr, Socket &sock)
{
	(void)fd;
	(void)server;
	(void)s_poll;
	(void)addr;
	(void)sock;
	return (0);
}
