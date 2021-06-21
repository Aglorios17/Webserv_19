/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   requesthandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elajimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/15 11:40:42 by elajimi           #+#    #+#             */
/*   Updated: 2021/06/17 16:47:05 by elajimi          ###   ########.fr       */
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
	Request request;

	printf("[POLLIN] read from %s(%d)\n", *fd == server ? "server" : "client", *fd);
	fflush(stdout);
	if (*fd != server)
	{
		while ((ret = recv(*fd, buffer, BUFFER_SIZE, 0)) > 0){}
		request.add(buffer);
		request.request_data();
		sock.set_request(request);
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

void clean_substring(std::string &main, std::string to_delete)
{
	size_t pos = main.find(to_delete);	
	if (pos != std::string::npos)
		main.erase(pos, to_delete.length());
}

void clean_path(std::string &path)
{
	clean_substring(path, "http://localhost:8080/");
}

void pollout_handler(int *fd, int server, struct poll* s_poll,
		struct sockaddr *addr, Socket &sock)
{
	printf("[POLLOUT] send to %s(%d)\n", *fd == server ? "server" : "client", *fd);
	fflush(stdout);

	if (*fd != server)
	{
		std::string source = sock.get_request().get_referer();
		clean_path(source);
		if (source.length() == 0)
			source = "index.html";
		source= "src/includes/static/" + source;
		send_html(*fd, &source[0]);

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
	printf("==========\n");
}

