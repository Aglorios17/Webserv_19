#include "includes/webserver.h"

void pollin_handler(int fd, int server, struct poll* s_poll,
		struct sockaddr *addr, Socket &sock)
{
	char buffer[BUFFER_SIZE];

	printf("read from %s(%d)\n", fd == server ? "server" : "client", fd);
	fflush(stdout);
	(void)s_poll;
	(void)addr;
	(void)sock;
	if (fd != server)
	{
	      recv(fd, buffer, BUFFER_SIZE, 0);
	      printf("%s", buffer);
	}
	else
	{
		printf("adding new connection.\n");
		add_connection(sock, addr, s_poll);
	}

	printf("==========\n");
	sleep(2);
}

void pollout_handler(int fd, int server, struct poll* s_poll,
		struct sockaddr *addr, Socket &sock)
{
	printf("read from %s(%d)\n", fd == server ? "server" : "client", fd);
	fflush(stdout);
	(void)s_poll;
	(void)sock;
	(void)addr;
	if (fd != server)
		send_html(fd, "src/includes/static/index.html");
	printf("==========\n");
	//sleep(2);
}

void poller_handler(int fd, int server, struct poll* s_poll,
		struct sockaddr *addr, Socket &sock)
{
	printf("%s CONNECTION INTERRUPTED\n", fd == server ? "SERVER" : "CLIENT");
	fflush(stdout);
	(void)s_poll;
	(void)sock;
	(void)addr;
	if (fd != server)
	{
	      close(fd);
	      delete_last(s_poll);
	      add_connection(sock, addr, s_poll);
	}
	printf("==========\n");
	//sleep(2);
}

