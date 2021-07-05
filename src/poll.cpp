/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   poll.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elajimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/07 16:08:18 by elajimi           #+#    #+#             */
/*   Updated: 2021/06/10 17:47:18 by elajimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/webserver.h"

/*
 * These functions will be part of a poll.cpp
 * handling all poll settings
 */

int init_poll_struct(struct poll* poll)
{
	poll->fds = NULL;
	poll->timeout = 0;
	poll->nfds = 0;
	return (0);
}

int		sizeof_fds(POLLFD* fds)
{
	int size;
	if (fds == NULL)
		return 0;
 	size = sizeof(*fds)/sizeof(fds[0]);

	return size;
}

POLLFD set_poll(int fd, int event, int FLAG)
{
	POLLFD	ret;
	
	memset(&ret, 0, sizeof(ret));
	ret.fd = fd;
	ret.revents = 0;
	ret.events = event;

	if (FLAG)
		fcntl(ret.fd, F_SETFL, O_NONBLOCK);
	return (ret);
}	

void	add_fd_to_poll(struct poll* poll, POLLFD fd)
{
	POLLFD * tmp = (POLLFD*)malloc(sizeof(POLLFD) * (poll->nfds + 1));//new POLLFD [poll->nfds];
	POLLFD tmp_fd; 

	memcpy(&tmp_fd, &fd, sizeof(fd));

	for (int i = 0 ; i < poll->nfds; i++)
		tmp[i] = poll->fds[i];

	tmp[poll->nfds] = fd;
	free(poll->fds);
	poll->fds = tmp;
	poll->nfds += 1;
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
