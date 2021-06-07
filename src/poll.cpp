/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   poll.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elajimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/07 16:08:18 by elajimi           #+#    #+#             */
/*   Updated: 2021/06/07 20:32:45 by elajimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/webserver.h"

/*
 * These functions will be part of a poll.cpp
 * handling all poll settings
 */

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
	//ret.events |= POLLIN;

	return (ret);
}	

void	add_fd_to_poll(POLLFD** fds, POLLFD fd)
{
	int			size = 0; 

	POLLFD * tmp = new POLLFD [size + 1];
	for (int i = 0 ; i < size; i++)
		tmp[i] = *fds[i];
	tmp[size] = fd;
	*fds = tmp;
	free(tmp);
}

