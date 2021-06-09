/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connect.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elajimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/08 16:40:44 by elajimi           #+#    #+#             */
/*   Updated: 2021/06/09 18:36:01 by elajimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

int add_connection(Socket &sock, struct sockaddr *addr, struct poll* s_poll)
{

	int	len;
	int sender;
	int optval = 1;

	len = sizeof((sockaddr_in*)addr);
	if ((sender = accept(sock.get_fd(),
			addr, (socklen_t*)&len)) < 0)
		exit (EXIT_FAILURE);

	setsockopt(sender, SOL_SOCKET, SO_REUSEADDR, &optval, 4);
	add_fd_to_poll(
					s_poll,
					set_poll(
							sender,
							POLLIN|POLLNVAL,
							O_NOFLAG));

	return sender;
}

void run_server(Socket &sock, struct sockaddr *addr, struct poll* s_poll)
{
	int		ret;
	int		fd;
	int		sender;
	char	buffer[BUFFER_SIZE] = {0};

	ret = 0;

	int listener = sock.get_fd();

	if (listen(sock.get_fd(), 1) < 0)//should add that in socket constructor
		exit (EXIT_FAILURE);

	sender = add_connection(sock, addr, s_poll);

	//========select
	//fd_set fds[s_poll->nfds];
	//for (int i = 0; i < s_poll->nfds; i++)
	//{
	//	FD_ZERO(&s_poll->fds[i].fd);
	//	FD_SET(s_poll->fds[i].fd, &fds[i]);
	//}
	//========

	while (1)
	{
		ret = poll(s_poll->fds, s_poll->nfds, -1);
		/*
		 * This while loop should be refactored into 
		 * a proper functin
		 */

		printf("...\n");
		fflush(stdout);

		for(int i = 0; i < s_poll->nfds ; i++)
		{
			fd = s_poll->fds[i].fd; 
			printf("sender %d\nfd %d\n", sender, fd);
			fflush(stdout);
			if ((s_poll->fds[i].revents & POLLIN) == POLLIN)
				/*data from read and sent to socket*/
			{
				//(void)listener;
				if (fd == listener && (s_poll->fds[i].revents & POLLHUP))
				{
					s_poll->fds[i].fd = -1;//disreagarding closed connection
					sender = add_connection(sock, addr, s_poll);
				}
				if (fd == sender)
				{
					read(fd, buffer, BUFFER_SIZE);
					send_html(fd, "src/includes/static/index.html");
				}
			}
		}
	}
}
