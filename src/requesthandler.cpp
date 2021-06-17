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
		while ((ret = recv(*fd, buffer, BUFFER_SIZE, 0)) > 0)
		printf("---%s---\n", buffer);
		fflush(stdout);
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

void clean_substring(std::string main, std::string to_delete)
{
	size_t pos = main.find(to_delete);	
	if (pos != std::string::npos)
		main.erase(pos, to_delete.length());
}

void clean_path(std::string path)
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
		std::string path = sock.get_request().get_referer();
		//clean_path(path);
		//std::cout<<"------->"<<path<<std::endl;
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

