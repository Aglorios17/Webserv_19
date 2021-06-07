/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elajimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/07 16:09:09 by elajimi           #+#    #+#             */
/*   Updated: 2021/06/07 20:33:48 by elajimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/webserver.h"

int		main()
{
	Socket				socket;	
	struct sockaddr_in*	address;/*refactoring this and fds into one struct*/
	POLLFD*				fds;

	fds = NULL;
	address = new struct sockaddr_in;

	add_fd_to_poll(
			&fds,
			set_poll(socket.get_fd(), POLLIN));

	configure(socket, address);

	bind(socket.get_fd(),
			(struct sockaddr *)address,
			sizeof(struct sockaddr_in));

	run_server(socket, (struct sockaddr*)address, &fds);
	return (0);
}
