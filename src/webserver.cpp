/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elajimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/07 16:09:09 by elajimi           #+#    #+#             */
/*   Updated: 2021/06/09 14:56:38 by elajimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/webserver.h"

int		main()
{
	Socket				socket;	
	struct sockaddr_in*		address;
	POLLFD*				fds;
	struct poll			*poll;

	fds = NULL;

	address = new struct sockaddr_in;

	poll = (struct poll*)malloc(sizeof(struct poll));
	init_poll_struct(poll);

	add_fd_to_poll(
					poll,
					set_poll(
							socket.get_fd(),
							POLLIN,
							O_NOFLAG));
	configure(socket, address);

	bind(socket.get_fd(),
			(struct sockaddr *)address,
			sizeof(struct sockaddr_in));

	run_server(socket, (struct sockaddr*)address, poll);
	return (0);
}
