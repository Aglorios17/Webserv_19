#include "includes/webserver.h"

/*
 * queue should be defined in the conf and store in a class
 */
# define QUEUE 3
# define BUFFER_SIZE 1000

//void send_header(int fd, int size)
//{
//	const char *s1 = "HTTP/1.1 200 OK\nContent-length: 0\n";
//	const char *s2 = "Content-Type: text/html\n\n";
//	send(fd, s1, strlen(s1), 0);
//	send(fd, s2, strlen(s2), 0);
//}
//
//void send_html(int fd, char *path)
//{
//	send_header(fd, get_file_size(path));
//}

void run_server(Socket &sock, struct sockaddr *addr)
{
	int fd;
	int ret;
	int len;
	char buffer[BUFFER_SIZE] = {0};

	len = sizeof((sockaddr_in*)addr);
	while (1)
	{
		if (listen(sock.get_fd(), QUEUE) < 0)
			exit (EXIT_FAILURE);
		if ((fd = accept(sock.get_fd(),
				addr, (socklen_t*)&len)) < 0)
			exit (EXIT_FAILURE);
		ret = read(fd, buffer, BUFFER_SIZE);
		//send_html(fd, "static/index.html");
	}
}
