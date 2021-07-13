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

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */

/*
 * 1: Parsing information in the conf
 * 2: defining a socket descriptor, using a socket()
 * 3: bind the socket to a network, using bind()
 * 4: listen using listen()
 * More info available in the README.md
 */

#include <iostream>
#include <cstdio>
#include <sstream>
#include <string>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
//#include <sys/epoll.h> /*epoll is not supported on macos*/
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/event.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <fstream>
#include <poll.h>
#include <time.h>
#include <ctime>
#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>

#include "socket.h"
#include "parser.hpp"
#include "request.hpp"
#include "cgi.hpp"

#define POLLFD struct pollfd
#define PORT 8080
#define MAX_FD 100 /*shoudl find it through limits.h or something*/
#define O_NOFLAG 0
#define BUFFER_SIZE 4096 
#define SET 0
#define GET 1

typedef struct	s_data
{
		char		*last;
		int		status;
		char		*buffer;
		struct sockaddr *ref;
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

void	run_server(Socket sock [] , struct sockaddr *addr, struct poll* poll);

/*
 * I/O File
 */

int	get_file_size(char const *path);
int	file2socket(int fd, char *path, Socket &sock, t_data *data);
std::string send_header(Socket &sock, int fd, int size, char *type, t_data *data);

/*
 * method 
 */

int method_error(int *fd, Socket &sock, t_data *data);
int method_get(int *fd, Socket &sock, t_data *data);
int method_post(int *fd, Socket &sock, t_data *data);
int method_delete(int *fd, Socket &sock, t_data *data);
void set_request(Request request, Socket &sock, char *buffer, t_data *data);
int apply_request(int *fd, Socket &sock, t_data *data);
void reset_sock_request(Socket &sock);

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

void		msleep(int tms);
void		clean_path(std::string &path);
bool		file_exists(char const* name);
std::string	*str_add_back(std::string *tab, std::string add);
int		*int_add_back(int *tab, int *add, int length, int size_add);
int		*int_in_tab(int *tab, int add, int size);
std::string	get_time(t_data *data);
std::string	bypass_tab(char *str);
int		hexa_to_decimal(std::string value);
std::string	get_extension(std::string file);
std::string	strtrim(std::string &s,char c);
int		get_server(Socket sock [],int **holdr, int mode);
int		is_server(int fd);
int		tab_len(int *tab);
void clean_substring(std::string &main, std::string to_delete);

/*
 * PRINT
 */

void print_request(std::string str);
void print_reponse(std::string str);

#endif
