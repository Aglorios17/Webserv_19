/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   requesthandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elajimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/15 11:40:42 by elajimi           #+#    #+#             */
/*   Updated: 2021/06/28 19:34:30 by elajimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/webserver.h"

//--------------Sock utils
void receive_data(int fd,Socket &sock)
{
	char buffer[BUFFER_SIZE];
	(void)sock;

	while(read(fd, buffer, BUFFER_SIZE) > 0)
		std::cout<<"["<<buffer<<"]"<<std::endl;
	return ;
}

void set_request(Request request, Socket &sock, char *buffer)
{
	request.add(buffer);
	request.request_data();
	sock.set_request(request);
}

int apply_method(POLLFD *poll, Socket &sock)
{
	if (sock.get_request().get_method() == "POST")
	{
		receive_data(poll->fd, sock);
		if (poll->revents&POLLOUT)
		{
			send_header(poll->fd, 0, NULL, 100);
			std::cout<<"done receiving"<<std::endl;
		}
		return 1;
	}
	return 0;
}
//------------------------

int pollin_handler(POLLFD *poll, int server, struct poll* s_poll,
		struct sockaddr *addr, Socket &sock)
{
	int ret;
	char buffer[BUFFER_SIZE];
	Request request;

	printf("[POLLIN] read from %s(%d)\n", poll->fd == server ? "server" : "client", poll->fd);
	fflush(stdout);
	if (poll->fd != server)
	{
		while ((ret = recv(poll->fd, buffer, BUFFER_SIZE, 0)) > 0){}

		printf("----------------\n");
		printf("Client Request:\n%s", buffer);
		printf("----------------\n\n");

		set_request(request, sock, buffer);
		if (apply_method(poll, sock))
			poller_handler(&poll->fd, server, s_poll, addr, sock);
	}
	else
	{
		printf("adding new connection.\n");
		add_connection(sock, addr, s_poll);
		msleep(50);
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
	(void)s_poll;
	(void)addr;

	if (*fd != server)
	{
		std::string source = sock.get_request().get_arg_method();
		clean_path(source);
		if (source.length() == 0)
			source = sock.get_parser().get_index();
		source= sock.get_parser().get_root() + source;
		send_html(*fd, &source[0], sock);
		poller_handler(fd, server, s_poll, addr, sock);
	}
	msleep(150);
	printf("==========\n");
}

void poller_handler(int *fd, int server, struct poll* s_poll,
		struct sockaddr *addr, Socket &sock)
{
	printf("%s CONNECTION INTERRUPTED\n", *fd == server ? "SERVER" : "CLIENT");
	fflush(stdout);
	(void)sock;
	(void)addr;
	if (*fd != server && s_poll->nfds > 1)
	{
	    close(*fd);
	    *fd = -1;
	    delete_last(s_poll);
	}
	msleep(50);
	printf("==========\n");
}
