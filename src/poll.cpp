/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   poll.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elajimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/07 16:08:18 by elajimi           #+#    #+#             */
/*   Updated: 2021/06/08 14:44:44 by elajimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/webserver.h"

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
 	size = sizeof(fds)/sizeof(fds[0]);

	return size;
}

POLLFD set_poll(int fd, int event)
{
	POLLFD ret;
	
	ret.fd = fd;
	ret.revents = 0;
	ret.events = event;
	return (ret);
}	

void	add_fd_to_poll(struct poll* poll, POLLFD fd)
{
	POLLFD * tmp = new POLLFD [poll->nfds + 1];
	for (int i = 0 ; i < poll->nfds; i++)
		tmp[i] = poll->fds[i];
	tmp[poll->nfds] = fd;
	poll->fds = tmp;
	poll->nfds += 1;
	free(tmp);
}

