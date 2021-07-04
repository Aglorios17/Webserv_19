/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elajimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/15 12:08:16 by elajimi           #+#    #+#             */
/*   Updated: 2021/06/28 16:09:35 by elajimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERVER_H
#define WEBSERVER_H

/*
 * 1: Parsing information in the conf
 * 2: defining a socket descriptor, using a socket()
 * 3: bind the socket to a network, using bind()
 * 4: listen using listen()
 * More info available in the README.md
 */

#include <iostream>
#include <string>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
//#include <sys/epoll.h> /*epoll is not supported on macos*/
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <fstream>
#include <poll.h>
#include <time.h>
#include <ctime>

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#include "socket.h"
#include "parser.hpp"
#include "request.hpp"

#define POLLFD struct pollfd
#define PORT 8080
#define MAX_FD 100 /*shoudl find it through limits.h or something*/
#define O_NOFLAG 0
#define BUFFER_SIZE 1024 

typedef struct	s_data
{
		char	*last;
		int		status;
		char	*buffer;
}				t_data;

bool	conf_is_valid(std::string &conf_path);
bool	domain_is_valid(int domain);
bool	type_is_valid(int type);
bool	protocol_is_valid(int protocol);
bool	port_is_valid(int port);
bool	is_valid(int domain, int type, int protocol, int port);

/*
 * These functions avoid mutation,
 * the conf is parsed and only the
 * corresponding string is returned.
 */

int	get_domain(std::string &conf_path);
int	get_type(std::string &conf_path);
int	get_protocol(std::string &conf_path);
void	*get_interface();

/*
 * CONFIGURE 
 */

void	configure(Socket &sock, struct sockaddr_in *addr);
void	configure_adress(Socket &sock, struct sockaddr_in *addr);

/*
 * CONNECT
 */

void	run_server(Socket &sock, struct sockaddr *addr, struct poll* poll);

/*
 * I/O File
 */

int	get_file_size(char const *path);
void	send_html(int fd, char *path, Socket &sock, t_data *data);
int	send_header(Socket &sock, int fd, int size, char *type, t_data *data);

/*
 * method 
 */
bool method_get(int *fd, int server, Socket &sock, char *request);
bool method_post(int *fd, int server, struct poll* s_poll, struct sockaddr *addr, Socket &sock);
bool method_delete(int *fd, int server, struct poll* s_poll, struct sockaddr *addr, Socket &sock);
void set_request(Request request, Socket &sock, char *buffer, t_data *data);
int apply_request(POLLFD *poll, Socket &sock, t_data *data);

/*
 * poll handlers
 */

/*
 * FD_STATUS refers to the fcntl SETFL
 * if set to 0, no flag is passed
 */

struct poll
{
	POLLFD *fds;
	int		nfds;
	int		timeout;
};

int	init_poll_struct(struct poll *poll);
int	sizeof_fds(POLLFD* fds);
POLLFD	set_poll(int fd, int event, int FD_STATUS);
void	add_fd_to_poll(struct poll* poll, POLLFD fd);



int pollin_handler(POLLFD *poll, int server, struct poll* s_poll,
	struct sockaddr *addr, Socket &sock, t_data *data);
void pollout_handler(int *fd, int server, struct poll* s_poll,
		struct sockaddr *addr, Socket &sock, t_data *data);
void	poller_handler(int *fd, int server, struct poll* s_poll,
	struct sockaddr *addr, Socket &sock);

int	add_connection(Socket &sock, struct sockaddr *addr,
	struct poll* s_poll);
void	delete_last(struct poll * poll);

/*
 * UTILS
 */

void	msleep(int tms);
void	clean_path(std::string &path);
bool	file_exists(char const* name);
std::string *str_add_back(std::string *tab, std::string add);
int *int_add_back(int *tab, int add);
std::string get_time(t_data *data);

#endif
