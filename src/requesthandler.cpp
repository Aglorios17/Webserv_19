#include "includes/webserver.h"

void pollin_handler(int fd, int server)
{
	char buffer[BUFFER_SIZE];

	printf("read from %s\n", fd == server ? "server" : "client");
	recv(fd, buffer, BUFFER_SIZE, 0);
	printf("%s", buffer);
	printf("==========\n");
}

void pollout_handler(int fd, int server)
{
	printf("write to %s\n", fd == server ? "server" : "client");
	send(fd, "", 0, 0);
	printf("==========\n");
}

void poller_handler(int fd, int server, struct poll* s_poll,
		struct sockaddr *addr, Socket &sock)
{
	printf("%s CONNECTION INTERRUPTED\n", fd == server ? "SERVER" : "CLIENT");
	if (fd != server)
	{
		close(fd);
		delete_last(s_poll);
		add_connection(sock, addr, s_poll);
	}
	printf("==========\n");
}

