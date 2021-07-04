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

#include "includes/webserver.h"

/*
 * queue should be defined in the conf and store in a class
 */
# define QUEUE 3

/*
 * ENV VARIABLES:
 *
 *	REQUEST_METHOD (GET/POST/DELETE)
 *	SERVER_PROTOCOL (HTTP/1.1)
 *	PATH_INFO (arg method)
 */

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
		case 404:
			return "HTTP/1.1 404 File Not Found\n";
		case 411:
			return "HTTP/1.1 411 Length Required\n";
		default:
			return "HTTP/1.1 200 OK\r\n";
	}
}

int	send_header(Socket &sock, int fd, int size, char* type, t_data *data)
{
	char const	*s1;
	std::string	buf;
	int		ret;
	std::string last = data->last;

	buf = get_header_message(data->status);
	//-----------------switch function
	buf += get_time(data);
	buf += "Server: " + sock.get_parser().get_server_name() + "\r\n"; 
//	if (last != "") /////////// pas ouf lol
//	{
//		std::string str(data->last);
//		buf += ("Last-Modified: " + str);
//	}
	if (type)
	{
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

	std::cout<<"------------"<<std::endl;
	std::cout<<"HTTP HEADER:"<<std::endl<<buf;
	std::cout<<"------------"<<std::endl;
	s1 = &buf[0];
	ret = send(fd, s1, strlen(s1), 0);
	return (ret);
}

std::string get_extension(std::string file)
{
	std::string type;
	std::size_t pos = file.find('.');

	if (pos == std::string::npos)
		return file;
	type = file.substr(pos+1);
//	std::cout<<"pos is: "<<pos<<std::endl;
//	std::cout<<"type is: "<<type<<std::endl;
	return type;
}

void set_env(Socket &sock, std::string path_info)
{
	const char *REQUEST_METHOD;
	const char *SERVER_PROTOCOL;
	const char *PATH_INFO;

	REQUEST_METHOD = sock.get_request().get_method().c_str();
	SERVER_PROTOCOL = sock.get_request().get_http_method().c_str();
	PATH_INFO = path_info.c_str();

	std::cout<<"-------\nCGI ENV VAR:\n"<<"REQUEST METHOD: "<<REQUEST_METHOD<<std::endl;
	std::cout<<"SERVER PROTOCOL: "<< SERVER_PROTOCOL<<std::endl;
	std::cout<<"PATH INFO: "<< PATH_INFO <<std::endl;

	setenv("REQUEST_METHOD", REQUEST_METHOD, 1);
	setenv("SERVER_PROTOCOL", SERVER_PROTOCOL, 1);
	setenv("PATH_INFO", PATH_INFO, 1);
}

void cgi_handler(std::string cgi, Socket &sock, std::string path_info)
{
	set_env(sock, path_info);
	std::cout<<"cgi------------------>"<<cgi.c_str()<<std::endl;
	int pid = 0;
	if ((pid = fork()) == 0)
	{
		system(cgi.c_str());//<-----fork 
		exit(0);	
	}
	char buffer[BUFFER_SIZE];
	std::cout<<"going in"<<std::endl;
	read(1, buffer, BUFFER_SIZE);
	std::cout<<"____________________cgi output: "<<buffer<<std::endl;
	return ;
}


void send_html(int fd, char *path, Socket &sock, t_data *data)
{
	const char *s1;
	std::string line;
	std::ifstream file;
	

	std::cout<<"\ncgi_extension: "<< sock.get_parser().get_cgi_path()<<std::endl;

	printf("FILE REQUESTED: %s\n", path);
	fflush(stdout);

	if (file_exists(path) == false)//check if file exists
	{
		printf("ERROR: FILE NOT FOUND\n");	
		fflush(stdout);
		data->status = 404;
		std::string error = sock.get_parser().get_root() + sock.get_parser().get_error_page();
		path = (char*)malloc(strlen(&error[0]) + 1);
		strcpy(path, &error[0]);
	}
	std::string s_path(path);
	std::string cgi = sock.get_parser().get_cgi_extension();

	std::string s_file = s_path.substr(s_path.find_last_of('/') + 1);

	std::string cgi_extension = cgi.substr(cgi.find_last_of('/') + 1);
	std::string cgi_path = sock.get_parser().get_cgi_path();


	if (get_extension(cgi_extension).compare(get_extension(s_file)) == 0)//check cgi
	{
		cgi_handler(cgi_path, sock, path);
		return ;
	}

	file.open(path);

	send_header(sock, fd, get_file_size(path), &get_extension(s_file)[0], data);

	while(std::getline(file, line))
	{
		s1 = &line[0];
		send(fd, s1, strlen(s1), 0);
	}
	file.close();
}

void direct_request(Socket &sock, struct sockaddr *addr, struct poll* s_poll, t_data *data)
{
	int		*fd;
	int		server;

	server = s_poll->fds[0].fd;
	for(int i = 0; i < s_poll->nfds ; i++)
	{
		fd = &s_poll->fds[i].fd;

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

void init_data(t_data *data)
{
	data->last = strdup("");
	data->buffer = strdup("no request");
	data->status = 0;
}

void run_server(Socket &sock, struct sockaddr *addr, struct poll* s_poll)
{
	int	ret;
	t_data data;

	init_data(&data);
	if (listen(sock.get_fd(), 1) < 0)
		exit (EXIT_FAILURE);
	while ((ret = poll(s_poll->fds, s_poll->nfds, sock.get_timeout())) >= 0)
	{
		direct_request(sock, addr, s_poll, &data);
		free(data.buffer);
		data.buffer = strdup("no request");
	}
	printf("ret--->%d\n", ret);
	fflush(stdout);
	free(data.last);
	free(data.buffer);
}
