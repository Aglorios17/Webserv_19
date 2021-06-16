/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   requesthandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elajimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/15 11:40:42 by elajimi           #+#    #+#             */
/*   Updated: 2021/06/16 17:18:25 by elajimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/webserver.h"

void msleep(int tms)
{
    struct timeval tv;
    tv.tv_sec  = tms / 1000;
    tv.tv_usec = (tms % 1000) * 1000;
    select (0, NULL, NULL, NULL, &tv);
}

int pollin_handler(int *fd, int server, struct poll* s_poll,
		struct sockaddr *addr, Socket &sock)
{
	int ret;
	char buffer[BUFFER_SIZE];

	printf("[POLLIN] read from %s(%d)\n", *fd == server ? "server" : "client", *fd);
	fflush(stdout);
	if (*fd != server)
	{
		while ((ret = recv(*fd, buffer, BUFFER_SIZE, 0)) > 0)
		printf("---%s---\n", buffer);
		fflush(stdout);
	}
	else
	{
		printf("adding new connection.\n");
		add_connection(sock, addr, s_poll);
		return 1;
	}
	printf("==========\n");
	return 0;
}

void pollout_handler(int *fd, int server, struct poll* s_poll,
		struct sockaddr *addr, Socket &sock)
{
	printf("[POLLOUT] send to %s(%d)\n", *fd == server ? "server" : "client", *fd);
	fflush(stdout);

	if (*fd != server)
	{
		send_html(*fd, "src/includes/static/index.html");
		poller_handler(fd, server, s_poll, addr, sock);
	}
	printf("==========\n");
	msleep(150);
}

void poller_handler(int *fd, int server, struct poll* s_poll,
		struct sockaddr *addr, Socket &sock)
{
	printf("%s CONNECTION INTERRUPTED\n", *fd == server ? "SERVER" : "CLIENT");
	fflush(stdout);
	(void)sock;
	(void)addr;
	if (*fd != server)
	{
	    close(*fd);
	    delete_last(s_poll);
		*fd = -1;
	}
	printf("==========\n");
}

