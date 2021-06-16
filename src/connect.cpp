/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connect.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elajimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/08 16:40:44 by elajimi           #+#    #+#             */
/*   Updated: 2021/06/16 17:14:19 by elajimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/webserver.h"

/*
 * queue should be defined in the conf and store in a class
 */
# define QUEUE 3


int	send_header(int fd, int size)
{
	char const	*s1;
	std::string	buf;
	int		ret;

	buf = "HTTP/1.0 OK\nContent-Type:text/html\nContent-Length: "
		+ std::to_string(size) + "\n\n";
	s1 = &buf[0];
	ret = send(fd, s1, strlen(s1), 0);
	return (ret);
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

void delete_last(struct poll * poll)
{
	POLLFD * tmp = (POLLFD*)malloc(sizeof(POLLFD) * (poll->nfds));

	for (int i = 0 ; i < poll->nfds - 1; i++)
		tmp[i] = poll->fds[i];
	delete poll->fds;
	poll->fds = tmp;
	poll->nfds--;
}

int add_connection(Socket &sock, struct sockaddr *addr, struct poll* s_poll)
{

	int len;
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
							POLLIN|
							POLLOUT|
							POLLHUP|
							POLLERR,
							O_NONBLOCK));

	return sender;
}
void direct_request(Socket &sock, struct sockaddr *addr, struct poll* s_poll)
{
		int		*fd;
		int		server;

		server = s_poll->fds[0].fd;
		for(int i = 0; i < s_poll->nfds ; i++)
		{
			fd = &s_poll->fds[i].fd;

			if (s_poll->fds[i].revents&POLLIN)
			{
				if (pollin_handler(fd, server, s_poll,
							addr, sock))
					return ;
			}

			else if (s_poll->fds[i].revents&POLLOUT)
			{
				pollout_handler(fd, server, s_poll,
							addr, sock);/*remove client*/
			}


			else if (s_poll->fds[i].revents&(POLLHUP|POLLERR))
			{
				poller_handler(fd, server, s_poll,
							addr, sock);/*sometimes*/
			}
		}
}

void run_server(Socket &sock, struct sockaddr *addr, struct poll* s_poll)
{
	int	ret;

	if (listen(sock.get_fd(), 1) < 0)
		exit (EXIT_FAILURE);


	while ((ret = poll(s_poll->fds, s_poll->nfds, sock.get_timeout())) >= 0)
		direct_request(sock, addr, s_poll);
	printf("ret--->%d\n", ret);
	fflush(stdout);
}
