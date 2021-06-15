/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   requesthandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elajimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/15 11:40:42 by elajimi           #+#    #+#             */
/*   Updated: 2021/06/15 14:47:43 by elajimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/webserver.h"

void pollin_handler(int *fd, int server, struct poll* s_poll,
		struct sockaddr *addr, Socket &sock)
{
	char buffer[BUFFER_SIZE];

	printf("read from %s(%d)\n", *fd == server ? "server" : "client", *fd);
	fflush(stdout);
	(void)s_poll;
	(void)addr;
	(void)sock;
	if (*fd != server)
	{
	      if (recv(*fd, buffer, BUFFER_SIZE, 0) == 0)
		  {
			poller_handler(fd, server, s_poll, addr, sock);
			*fd = -1;
		  }
	      printf("__%s__\n", buffer);
		  fflush(stdout);
	}
	else
	{
		printf("adding new connection.\n");
		while (recv(server, buffer, BUFFER_SIZE, 0) > 0)
			printf("~~%s~~\n", buffer);
		add_connection(sock, addr, s_poll);
	}

	printf("==========\n");
}

void pollout_handler(int *fd, int server, struct poll* s_poll,
		struct sockaddr *addr, Socket &sock)
{
	char buffer[BUFFER_SIZE];

	memset(&buffer, 0, BUFFER_SIZE);
	printf("send to %s(%d)\n", *fd == server ? "server" : "client", *fd);
	fflush(stdout);

	if (*fd != server)
	{
		send_html(*fd, "src/includes/static/index.html");

		int ret = recv(server, buffer, BUFFER_SIZE, 0);
		
	    printf("__%s__\n", buffer);
		fflush(stdout);

		if (ret == 0)
			poller_handler(fd, server, s_poll, addr, sock);
		*fd = -1;
	}
	printf("==========\n");
}

void poller_handler(int *fd, int server, struct poll* s_poll,
		struct sockaddr *addr, Socket &sock)
{
	printf("%s CONNECTION INTERRUPTED\n", *fd == server ? "SERVER" : "CLIENT");
	fflush(stdout);
	(void)s_poll;
	(void)sock;
	(void)addr;
	if (*fd != server)
	{
	    close(*fd);
	    delete_last(s_poll);
	    //add_connection(sock, addr, s_poll);
		*fd = -1;
	}
	printf("==========\n");
}

