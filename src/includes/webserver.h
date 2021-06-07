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
//#include <sys/epoll.h> /*epoll is not supported on macosx*/
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <fstream>

#include "socket.h"
#include "parser.hpp"

#define PORT 8080


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

void	run_server(Socket &sock, struct sockaddr *addr);

/*
 * I/O File
 */

int	get_file_size(char const *path);

#endif
