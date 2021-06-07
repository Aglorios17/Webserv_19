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

void run_server(Socket &sock, struct sockaddr *addr, POLLFD** fds)
{
	/*
	 * It is to be noted that fd multiplexing
	 * will be implemented.
	 * So far, we can see that we have an 
	 * fd for our socket and an fd for 
	 * any new accepted connection.
	 */

	int		fd;
	int		ret;
	int		len;
	char	buffer[BUFFER_SIZE] = {0};

	len = sizeof((sockaddr_in*)addr);
	if (listen(sock.get_fd(), QUEUE) < 0)
		exit (EXIT_FAILURE);
	if ((fd = accept(sock.get_fd(),
			addr, (socklen_t*)&len)) < 0)
		exit (EXIT_FAILURE);
	add_fd_to_poll(
					fds,
					set_poll(fd, POLLIN));
	while (1)
	{
			ret = read(fd, buffer, BUFFER_SIZE);
			int pret = poll(*fds, 3, sock.get_timeout()); /*find out where to put this thing*/
			if (pret == 0)
				send_html(fd, "src/includes/static/error.html");
			else 
				send_html(fd, "src/includes/static/index.html");
	}
}
