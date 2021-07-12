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

#include "../includes/webserver.h"

void receive_data(int fd,Socket &sock)
{
	char buffer[BUFFER_SIZE];
	(void)sock;

	while(read(fd, buffer, BUFFER_SIZE) > 0)
		std::cout<<"["<<buffer<<"]"<<std::endl;
	return ;
}

void reset_sock_request(Socket &sock)
{
	Request request;
	sock.set_request(request);
}

void set_request(Request request, Socket &sock, char *buffer, t_data *data)
{
	request.add(buffer);
	request.request_data();

	sock.set_request(request);
	if ((data->status = sock.get_request().get_status()) != 200)
		return ;
	if (sock.get_request().get_content_length() > sock.get_parser().get_client_max_body_size())
		data->status = 413;
}

int apply_request(int *fd, Socket &sock, t_data *data)
{
	int request_port = sock.get_request().get_port();
	int server_port =  sock.get_port(); 	

	if (request_port != server_port)
	{
		reset_sock_request(sock);
		return 0;
	}
	if (data->status != 200)
		return (method_error(fd, sock, data));
	if (sock.get_request().get_method() == "POST")
		return (method_post(fd, sock, data));
	else if (sock.get_request().get_method() == "GET")
		return (method_get(fd, sock, data));
	else if (sock.get_request().get_method() == "DELETE")
		return (method_delete(fd, sock, data));
	return 0;
}

int pollin_handler(POLLFD *poll, int server, struct poll* s_poll,
		struct sockaddr *addr, Socket &sock, t_data *data)
{
	int ret;
	uint8_t buffer[BUFFER_SIZE + 1];

	Request request;

	is_server(poll->fd);
	if (poll->fd != server)
	{
		while ((ret = recv(poll->fd, buffer, BUFFER_SIZE, 0)) > 0)
		{
			if (buffer[ret - 1]  == '\n')
			{
				buffer[ret] = '\0';
				break ;
			}
		}
		std::string tmp((char*)buffer);	
		data->buffer = (char*)tmp.c_str();
	 	set_request(sock.get_request(), sock, data->buffer, data);
	}
	else
	{
		printf("adding new connection.\n");
		add_connection(sock, addr, s_poll);
		msleep(50);
		return 1;
	}
	return 0;
}

void pollout_handler(int *fd, int server, struct poll* s_poll,
		struct sockaddr *addr, Socket &sock, t_data *data)
{
	if (*fd != server)
	{
		if (apply_request(fd, sock, data))
			poller_handler(fd, server, s_poll, addr, sock);
	}
	msleep(150);
}

void poller_handler(int *fd, int server, struct poll* s_poll,
		struct sockaddr *addr, Socket &sock)
{
	(void)sock;
	(void)addr;
	if (*fd != server && s_poll->nfds > 1)
	{
	    close(*fd);
	    *fd = -1;
	    delete_last(s_poll);
	}
	msleep(50);
}
