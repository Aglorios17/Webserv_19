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


/*
 *  1. check if file needs to be run with CGI
 *  2. Set Env variables using setenv
 *  3. fork and run cgi with env 
 */

bool	file_exists(char const* name)
{

	printf("INDEX QUERY: [%s]\n", name);
	fflush(stdout);

	std::ifstream f(name);
	return f.good();
}

int	send_header(int fd, int size, char* type, int err)
{
	char const	*s1;
	std::string	buf;
	int		ret;

	std::cout<<"err: "<<err<<std::endl;
	if (err == 201)
		buf = "HTTP/1.1 201 Created\n";
	else if (err == 100)
		buf = "HTTP/1.1 100 Continue\n";
	else if (err == 411)
		buf = "HTTP/1.1 411 Length Required\n";
	else if (err == 202)
		buf = "HTTP/1.1 202 Accepted\n";
	else if (err == 203)
		buf = "HTTP/1.1 203 Non-Authoritative Information\n";
	else if (err == 204)
		buf = "HTTP/1.1 204 No Content\n";
	else if (err == 206)
		buf = "HTTP/1.1 206 Partial Content\n";
	else if (err == 404)
		buf = "HTTP/1.1 404 File Not Found\n";
	else
		buf = "HTTP/1.1 200 OK\n";

	if (type)
	{
		if (strcmp(type, "html") == 0)
			buf += "Content-Type: text/html\n";
		else 
			buf += "Content-Type: image/jpg\n";
	}
	buf += "Cache-Control: no-store\n";
	buf += "Content-Length: " + std::to_string(size) + "\n\n";

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
	std::cout<<"pos is: "<<pos<<std::endl;
	std::cout<<"type is: "<<type<<std::endl;
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


void send_html(int fd, char *path, Socket &sock)
{
	const char *s1;
	int err = 0;
	std::string line;
	std::ifstream file;
	

	std::cout<<"\ncgi_extension: "<< sock.get_parser().get_cgi_path()<<std::endl;

	printf("FILE REQUESTED: %s\n", path);
	fflush(stdout);

	if (file_exists(path) == false)//check if file exists
	{
		printf("ERROR: FILE NOT FOUND\n");	
		fflush(stdout);
		err = 404;
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

	send_header(fd, get_file_size(path), &get_extension(s_file)[0], err);

	while(std::getline(file, line))
	{
		s1 = &line[0];
		send(fd, s1, strlen(s1), 0);
	}
	file.close();
}

void delete_last(struct poll * poll)
{
	POLLFD * tmp = (POLLFD*)malloc(sizeof(POLLFD) * (poll->nfds));

	for (int i = 0 ; i < poll->nfds - 1; i++)
		tmp[i] = poll->fds[i];
	delete poll->fds;
	poll->fds = tmp;
	poll->nfds--;
}

int add_connection(Socket &sock, struct sockaddr *addr, struct poll* s_poll)
{

	int len;
	int sender;
	int optval = 1;

	len = sizeof((sockaddr_in*)addr);
	if ((sender = accept(sock.get_fd(),
			addr, (socklen_t*)&len)) < 0)
		exit (EXIT_FAILURE);

	setsockopt(sender, SOL_SOCKET, SO_REUSEADDR, &optval, 4);

	add_fd_to_poll(
					s_poll,
					set_poll(
							sender,
							POLLIN|
							POLLOUT|
							POLLHUP|
							POLLERR,
							O_NONBLOCK));

	return sender;
}
void direct_request(Socket &sock, struct sockaddr *addr, struct poll* s_poll)
{
		int		*fd;
		int		server;

		server = s_poll->fds[0].fd;
		for(int i = 0; i < s_poll->nfds ; i++)
		{
			fd = &s_poll->fds[i].fd;

			if (s_poll->fds[i].revents&POLLIN)
				pollin_handler(&s_poll->fds[i], server, s_poll,
							addr, sock);
			else if (s_poll->fds[i].revents&POLLOUT)
				pollout_handler(fd, server, s_poll,
							addr, sock);
			else if (s_poll->fds[i].revents&(POLLHUP|POLLERR))
				poller_handler(fd, server, s_poll,
							addr, sock);
		}
}

void run_server(Socket &sock, struct sockaddr *addr, struct poll* s_poll)
{
	int	ret;

	if (listen(sock.get_fd(), 1) < 0)
		exit (EXIT_FAILURE);


	while ((ret = poll(s_poll->fds, s_poll->nfds, sock.get_timeout())) >= 0)
		direct_request(sock, addr, s_poll);
	printf("ret--->%d\n", ret);
	fflush(stdout);
}
