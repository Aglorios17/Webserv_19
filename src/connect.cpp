/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connect.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elajimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/08 16:40:44 by elajimi           #+#    #+#             */
/*   Updated: 2021/06/28 19:34:07 by elajimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/webserver.h"

std::string get_header_message(int status)
{
	switch (status)
	{
		case 100:
			return "HTTP/1.1 100 Continue\n";
		case 201:
			return "HTTP/1.1 201 Created\n";
		case 202:
			return "HTTP/1.1 202 Accepted\n";
		case 203:
			return "HTTP/1.1 203 Non-Authoritative Information\n";
		case 204:
			return "HTTP/1.1 204 No Content\n";
		case 206:
			return "HTTP/1.1 206 Partial Content\n";
		case 400:
			return "HTTP/1.1 400 Bad Request\n";
		case 404:
			return "HTTP/1.1 404 File Not Found\n";
		case 405:
			return "HTTP/1.1 405 Method Not Allowed\n";
		case 408:
			return "HTTP/1.1 408 Request Time-out\n";
		case 411:
			return "HTTP/1.1 411 Length Required\n";
		case 413:
			return "HTTP/1.1 413 Request Entity Too Large\n";
		case 415:
			return "HTTP/1.1 415 Unsupported Media Type\n";
		case 500:
			return "HTTP/1.1 500 Internal Server Error\n";
		case 501:
			return "HTTP/1.1 501 Not Implemented\n";
		case 505:
			return "HTTP/1.1 505 HTTP Version not supported\n";
		default:
			return "HTTP/1.1 200 OK\r\n";
	}
}

std::string send_header(Socket &sock, int fd, int size, char* type, t_data *data)
{
	char const	*s1;
	std::string	buf;
	(void)fd;

	buf = get_header_message(data->status);
	buf += get_time(data);
	if (sock.get_parser().get_server_name().size())
		buf += "Server: " + sock.get_parser().get_server_name() + "\r\n"; 
	if (type)
	{
		if (strcmp(type, "text") == 0)
			buf += "Content-Type: plain/text\r\n";
		if (strcmp(type, "php") == 0)
			buf += "Content-Type: text/html\r\n";
		else if (strcmp(type, "jpg") == 0)
			buf += "Content-Type: image/jpg\r\n";
		else if (strcmp(type, "png") == 0)
			buf += "Content-Type: image/png\r\n";
		else if (strcmp(type, "gif") == 0)
			buf += "Content-Type: image/gif\r\n";
		else if (strcmp(type, "jpeg") == 0)
			buf += "Content-Type: image/jpeg\r\n";
		else if (strcmp(type, "ico") == 0)
			buf += "Content-Type: image/x-icon\r\n";
		else
			buf += "Content-Type: text/html\r\n";
	}
	//-----------------------------------
	buf += "Cache-Control: no-store\r\n";
	buf += "Content-Length: " + std::to_string(size) + "\r\n\r\n";

	s1 = &buf[0];
	return (s1);
}

void set_env(Socket &sock, std::string path_info)
{
	const char *REQUEST_METHOD;
	const char *SERVER_PROTOCOL;
	const char *PATH_INFO;

	REQUEST_METHOD = sock.get_request().get_method().c_str();
	SERVER_PROTOCOL = sock.get_request().get_http_method().c_str();
	PATH_INFO = path_info.c_str();

	setenv("REQUEST_METHOD", REQUEST_METHOD, 1);
	setenv("SERVER_PROTOCOL", SERVER_PROTOCOL, 1);
	setenv("PATH_INFO", PATH_INFO, 1);
}

std::string get_good_path(Socket &sock, char *path, t_data *data)
{
	if (file_exists(path) == false)
	{
		data->status = 404;
		std::string error =
			sock.get_parser().get_root() +
			sock.get_parser().get_error_page();
		path = &error[0];
	}
	return path;
}

int file2socket(int fd, char *path, Socket &sock, t_data *data)
{
	std::string line;
	std::string s_path;
	std::string s1;
	int ret = 1;

	s_path = get_good_path(sock, path, data);
	path = &s_path[0];

	std::string s_file = s_path.substr(s_path.find_last_of('/') + 1);
	std::string extension = get_extension(s_file);
	std::ifstream file; 
	file.open(path,  std::ios::out | std::ios::binary | std::ios::ate);

	if (file.is_open())
	{
		std::string mem;
		unsigned long size = file.tellg();

		file.seekg(0);
		while(!file.eof())
		{
			char tmp = file.get();
			mem += tmp;
		}
		file.close();
		s1 = send_header(sock, fd, size, &get_extension(s_file)[0], data);
		size += s1.length() + 4;
		s1 += mem;
		s1 += "\r\n\r\n";
	}
	ret = send(fd, &s1[0], strlen(&s1[0]), 0);
	print_reponse(s1);
	file.close();
	return (ret);
}

void direct_request(Socket &sock, struct sockaddr *addr, struct poll* s_poll, t_data *data)
{
	int		*fd;
	int		server;

	server = sock.get_fd();

	for(int i = 0; i < s_poll->nfds ; i++)
	{
		fd = &s_poll->fds[i].fd;

		if (!is_server(*fd) || server == *fd)
		{
			if (s_poll->fds[i].revents&POLLIN)
				pollin_handler(&s_poll->fds[i], server, s_poll,
							addr, sock, data);
			else if (s_poll->fds[i].revents&POLLOUT)
				pollout_handler(fd, server, s_poll,
							addr, sock, data);
			else if (s_poll->fds[i].revents&(POLLHUP|POLLERR))
				poller_handler(fd, server, s_poll,
							addr, sock);
		}
	}
}

t_data	*initglobal(void)
{
	static t_data	data;

	return (&data);
}

void	signalhandler(int signum)
{
	t_data		*data;

	std::cout << GREEN << "\n=====================================" << RESET << std::endl;
	std::cout << GREEN << "====== EXIT WEBSERVER THANKS ! ======" << RESET << std::endl;
	std::cout << GREEN << "=====================================\n" << RESET << std::endl;
	data = initglobal();
	(void)signum;
	free(data->ref);
	free(data->last);
	exit(0);
}

void init_data(t_data *data)
{
	data->last = strdup("");
	data->buffer = (char*)"no request";
	data->status = 0;
}

void run_server(Socket sock [] , struct sockaddr *addr, struct poll* s_poll)
{
	int	ret;
	int	nport;
	t_data *data;

	nport = sock[0].get_parser().get_nport();
	int timeout = sock[0].get_timeout();

	data = initglobal();
	data->ref = addr;
	init_data(data);
	signal(SIGINT, signalhandler);
	while ((ret = poll(s_poll->fds, s_poll->nfds, timeout)) >= 0)
	{
		for (int i = 0; i < nport; i++)
		{
			direct_request(sock[i], addr, s_poll, data);
			data->buffer = (char*)"no request";
		}
		if (nport < 4)
			msleep(10);
	}
	free(data->last);
}
