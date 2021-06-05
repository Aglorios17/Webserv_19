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
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <poll.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include "Socket.h"

#define PORT 8080


bool	conf_is_valid(std::string &conf_path);
bool	domain_is_valid(int domain);
bool	type_is_valid(int type);
bool	protocol_is_valid(int protocol);
bool	is_valid(int domain, int type, int protocol);

/*
 * These functions avoid mutation,
 * the conf is parsed and only the
 * corresponding string is returned.
 */

int	get_domain(std::string &conf_path);
int	get_type(std::string &conf_path);
int	get_protocol(std::string &conf_path);

#endif
