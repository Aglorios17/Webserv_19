/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   requesthandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elajimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/15 11:40:42 by elajimi           #+#    #+#             */
/*   Updated: 2021/06/28 19:31:38 by elajimi          ###   ########.fr       */
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

void receive_data(int fd,Socket &sock)
{
	char buffer[BUFFER_SIZE];
	(void)sock;

	while(read(fd, buffer, BUFFER_SIZE) > 0)
		std::cout<<"["<<buffer<<"]"<<std::endl;
	return ;
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
		printf("----------------\n");
		fflush(stdout);
		printf("Client Request:\n%s", buffer);
		fflush(stdout);
		printf("----------------\n\n");
		fflush(stdout);
		request.add(buffer);
		request.request_data();
		sock.set_request(request);
		if (sock.get_request().get_method() == "POST")
		{
			receive_data(*fd, sock);
			char* type = (char*)"html";
			(void)type;
			send_header(*fd, 0, NULL, 411);
			std::cout<<"done receiving"<<std::endl;
			poller_handler(fd, server, s_poll, addr, sock);
		}
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

//------Move to a util*.cpp
void clean_substring(std::string &main, std::string to_delete)
{
	size_t pos = main.find(to_delete);	
	if (pos != std::string::npos)
		main.erase(pos, to_delete.length());
}

void clean_path(std::string &path)
{
	clean_substring(path, "/");
}
//----------------



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
