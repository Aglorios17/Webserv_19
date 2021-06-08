/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connect.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elajimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/08 16:40:44 by elajimi           #+#    #+#             */
/*   Updated: 2021/06/08 17:56:30 by elajimi          ###   ########.fr       */
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

	int		ret;
	int		fd;
	int		sock_fd;
	int		len;
	char	buffer[BUFFER_SIZE] = {0};

	ret = 0;
	len = sizeof((sockaddr_in*)addr);

	int listener = sock.get_fd();
	if (listen(sock.get_fd(), QUEUE) < 0)
		exit (EXIT_FAILURE);

	if ((sock_fd = accept(sock.get_fd(),
			addr, (socklen_t*)&len)) < 0)
		exit (EXIT_FAILURE);

	add_fd_to_poll(
					s_poll,
					set_poll(
							sock_fd,
							POLLIN,
							NO_STATUS_FLAG));

	while ((ret = poll(s_poll->fds, s_poll->nfds, sock.get_timeout())) != -1)
	{

		/*
		 * This while loop should be refactored into 
		 * a proper functin
		 */

		printf("...\n");
		fflush(stdout);

		for(int i = 0; i < s_poll->nfds ; i++)
		{
			fd = s_poll->fds[i].fd; 

			if ((s_poll->fds[i].revents & POLLIN) == POLLIN) /*data from read and sent to socket*/
			{
				if (fd == listener)
				{
					printf("adding new connection\n");
					add_fd_to_poll(
									s_poll,
									set_poll(
											accept(
												listener,
												addr, (socklen_t*)&len),
											POLLIN,
											NO_STATUS_FLAG));
				}
				printf("read from fd:%d and send data\n", s_poll->fds[1].fd);
				fflush(stdout);

				read(fd, buffer, BUFFER_SIZE);
				send_html(fd, "src/includes/static/index.html");

				printf("Done\n");
				fflush(stdout);
			}
			else if (ret == 0) 
			{
				printf("timeout was reached\n");
				printf("-->fd:    %d\n", fd);	
				printf("-->revent:%d\n", s_poll->fds[i].revents);
				printf("-->nfds:  %d\n", i);
			
			}
			else
				printf("return event for fd: %d, was %d\n", s_poll->fds[i].fd,
					   	s_poll->fds[i].revents);
		}
		usleep(500);
	}
}
