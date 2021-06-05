#include "includes/webserver.h"

Socket::Socket() :
       	_domain(AF_INET), _type(SOCK_STREAM),
	_protocol(0), _port(8080), _interface(INADDR_ANY)
{
	is_valid(_domain, _type, _protocol, _port);
	_socket_fd = socket(_domain, _type, _protocol);
}

Socket::Socket(int domain, int type, int protocol,
	       	int port, int interface) : 
	_domain(domain), _type(type),
	_protocol(protocol), _port(port), _interface(interface)
{
	is_valid(_domain, _type, _protocol, _port);
	_socket_fd = socket(_domain, _type, _protocol);
}

int Socket::get_domain(){return _domain;}

int Socket::get_type(){return _type;}

int Socket::get_protocol(){return _protocol;}

int Socket::get_port(){return _port;}

int Socket::get_interface(){return _interface;}

int Socket::get_fd(){return _socket_fd;}


Socket::~Socket(){}
