#include "includes/webserver.h"
/*
 * queue should be defined in the conf and store in a class
 */
# define QUEUE 3
# define BUFFER_SIZE 1000


void send_header(int fd, int size)
{
	char const *s1;
	std::string buf;

	buf = "HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length: " + std::to_string(size) + "\n\n";
	s1 = &buf[0];
	send(fd, s1, strlen(s1), 0);
}

void send_html(int fd, const char *path)
{
	const char *s1;
	std::string line;
	std::ifstream file(path);

	if (file.is_open())
	{

		send_header(fd, get_file_size(path));
		while(std::getline(file, line))
		{
			s1 = &line[0];
			send(fd, s1, strlen(s1), 0);
		}
		file.close();
	}
}

void run_server(Socket &sock, struct sockaddr *addr, struct poll* s_poll)
{
	/*
	 * fd[0] is the socket fd
	 *
	 * fd[1] is the connection
	 * with which the socket is made
	 * 
	 * Added fd will be for additional
	 * connections 
	 * 
	 */

	int		fd;
	int		len;
	char	buffer[BUFFER_SIZE] = {0};

	len = sizeof((sockaddr_in*)addr);

	if (listen(sock.get_fd(), QUEUE) < 0)
		exit (EXIT_FAILURE);

	if ((fd = accept(sock.get_fd(),
			addr, (socklen_t*)&len)) < 0)
		exit (EXIT_FAILURE);

	add_fd_to_poll(
					s_poll,
					set_poll(fd, POLLIN));

	while (poll(s_poll->fds, s_poll->nfds, sock.get_timeout()) != -1)
	{
		printf("...\n");
		fflush(stdout);

		if (s_poll->fds[1].revents & POLLIN) /*data from read and sent to socket*/
		{
			printf("read from fd:%d and send data\n", s_poll->fds[1].fd);
			fflush(stdout);

			read(s_poll->fds[1].fd, buffer, BUFFER_SIZE);
			send_html(s_poll->fds[1].fd, "src/includes/static/index.html");

			printf("Done\n");
			fflush(stdout);
		}
		if (s_poll->fds[0].revents & POLLIN)
		{
			printf("data recieved");
			fflush(stdout);
		}
		//	read(s_poll->fds[].fd, buffer, BUFFER_SIZE);
		//	send_html(fd, "src/includes/static/error.html");
			//if (pret == 0)
			//else 
	}
}
