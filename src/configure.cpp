/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configure.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elajimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/07 16:10:11 by elajimi           #+#    #+#             */
/*   Updated: 2021/06/07 16:10:11 by elajimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/webserver.h"

void configure_adress(Socket &sock, struct sockaddr_in *addr)
{
	addr->sin_family = sock.get_domain();
	addr->sin_port = htons(sock.get_port());
	addr->sin_addr.s_addr = htonl(sock.get_interface());
}

void configure(Socket &sock, struct sockaddr_in *addr)
{
	int opt = 1;
	setsockopt(sock.get_fd(), SOL_SOCKET, SO_KEEPALIVE, &opt, 4);
	configure_adress(sock, addr);
}
