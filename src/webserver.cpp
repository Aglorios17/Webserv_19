/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elajimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/07 16:09:09 by elajimi           #+#    #+#             */
/*   Updated: 2021/06/08 14:40:56 by elajimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/webserver.h"

int		main()
{
	Socket				socket;	
	struct sockaddr_in*	address;/*refactoring this and fds into one struct*/
	POLLFD*				fds;
	struct poll			poll;

	fds = NULL;
	address = new struct sockaddr_in;

	init_poll_struct(&poll);

	add_fd_to_poll(
					&poll,
					set_poll(socket.get_fd(), POLLIN));

	configure(socket, address);

	bind(socket.get_fd(),
			(struct sockaddr *)address,
			sizeof(struct sockaddr_in));

	run_server(socket, (struct sockaddr*)address, &poll);

	return (0);
}
