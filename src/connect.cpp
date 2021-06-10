/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connect.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elajimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/08 16:40:44 by elajimi           #+#    #+#             */
/*   Updated: 2021/06/10 18:29:41 by elajimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/webserver.h"
/*
 * queue should be defined in the conf and store in a class
 */
# define QUEUE 3
# define BUFFER_SIZE 10000


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

		printf("1[[[fd=>%d]]\n", fd);
		fflush(stdout);
		send_header(fd, get_file_size(path));
		printf("2[[[fd=>%d]]\n", fd);
		fflush(stdout);
		while(std::getline(file, line))
		{
			s1 = &line[0];
			send(fd, s1, strlen(s1), 0);
			printf("3[[[fd=>%d]]\n", fd);
			fflush(stdout);
		}
		file.close();
			printf("4[[[fd=>%d]]\n", fd);
			fflush(stdout);
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
							POLLIN|POLLOUT|POLLWRBAND|POLLHUP,
							O_NONBLOCK));

	return sender;
}

void run_server(Socket &sock, struct sockaddr *addr, struct poll* s_poll)
{
	int		ret;
	int		fd;
	char	buffer[BUFFER_SIZE] = {0};

	if (listen(sock.get_fd(), 1) < 0)
		exit (EXIT_FAILURE);

	add_connection(sock, addr, s_poll);

	while ((ret = poll(s_poll->fds, s_poll->nfds, 1000000)) != -1)
	{
		printf("...\n");
		fflush(stdout);

		int fd_sock = s_poll->fds[0].fd;
		//int fd_subsock = s_poll->fds[1].fd;

		for(int i = 0; i < s_poll->nfds ; i++)
		{
			fd = s_poll->fds[i].fd; 
			
			if ((s_poll->fds[i].revents & POLLIN) == POLLIN)
			{
				if (fd == fd_sock)
				{
					printf("read from sock \n");
					fflush(stdout);
					ret = recv(fd, buffer, BUFFER_SIZE, 0);
					printf("%s", buffer);
					printf("==========\n");
					//sleep(2);
				}
				if (fd != fd_sock)
				{
					printf("read from subsock\n");
					fflush(stdout);
					recv(fd, buffer, BUFFER_SIZE, 0);
					printf("%s", buffer);
					printf("==========\n");
					//sleep(2);
				}
			}
			if ((s_poll->fds[i].revents & POLLOUT) == POLLOUT)
			{
				if (fd == fd_sock)
				{
					printf("writing to sock\n");
					fflush(stdout);
					printf("==========\n");
					//sleep(2);
				}
				if (fd != fd_sock)
				{
					printf("writing to subsock htlml\n");
					fflush(stdout);

					send_html(s_poll->fds[i].fd, "src/includes/static/index.html");
					//s_poll->fds[i].fd;
					close(fd);
					add_connection(sock, addr, s_poll);
					printf("==========\n");
					fflush(stdout);
				}
			}
			if ((s_poll->fds[i].revents & POLLHUP) == POLLHUP)
			{
				if (fd == fd_sock)
				{
					printf("SOCK CONNECTION INTERRUPTEd\n");
					fflush(stdout);
					exit(0);
				}
				if (fd != fd_sock)
				{
					printf("SUBSOCK CONNECTION INTERRUPTEd\n");
					fflush(stdout);
					exit(0);
				}
			}
		for (int i = 0; i < s_poll->nfds; i++)
		{
			printf("fd%d -> %d\n", i, s_poll->fds[i].fd);
			fflush(stdout);
		}
			//if ((s_poll->fds[i].revents & POLLIN) == POLLIN)
			//	/*data from read and sent to socket*/
			//{
			//	recv(fd, buffer, BUFFER_SIZE, 0); 
			//	printf("[%s]\n", buffer);
			//	fflush(stdout);
			//}
			//if ((s_poll->fds[i].revents & POLLOUT) == POLLOUT)
			//{
			//		//send_html(fd, "src/includes/static/index.html");

			//		for (int i = 0; i < s_poll->nfds; i++)
			//		{
			//			printf("fd%d -> %d\n", i, s_poll->fds[i].fd);
			//			fflush(stdout);
			//		}
			//}
			
			printf("done\n");
			printf("~~~~~~~~~~~~~~~~~~~\n");
			fflush(stdout);
			//sleep(1);
		}
	}
}
