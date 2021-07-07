#include "../includes/webserver.h"

Socket::Socket() :
       	_domain(AF_INET), _type(SOCK_STREAM),
	_protocol(0), _port(8080), _interface(INADDR_ANY),
	_timeout(1000)
{
	//int optval = 1;

	//is_valid(_domain, _type, _protocol, _port);
	//_socket_fd = socket(_domain, _type, _protocol);
	//setsockopt(_socket_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(optval));
}

void Socket::set_sock(int domain, int type, int protocol,
	       	int port, int interface, int timeout)
{
	int optval = 1;

	_domain = domain;
	_type = type;
	_protocol = protocol;
	_port = port;
	_interface = interface;
	_timeout = timeout;

	is_valid(_domain, _type, _protocol, _port);
	_socket_fd = socket(_domain, _type, _protocol);
	setsockopt(_socket_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int));
}

Socket::Socket(int domain, int type, int protocol,
	       	int port, int interface, int timeout) : 
	_domain(domain), _type(type),
	_protocol(protocol), _port(port), _interface(interface), 
	_timeout(timeout)
{
	int optval = 1;

	is_valid(_domain, _type, _protocol, _port);
	_socket_fd = socket(_domain, _type, _protocol);
	setsockopt(_socket_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int));
}

int Socket::get_domain(){return _domain;}

int Socket::get_type(){return _type;}

int Socket::get_protocol(){return _protocol;}

int Socket::get_port(){return _port;}

int Socket::get_interface(){return _interface;}

int Socket::get_fd(){return _socket_fd;}

int Socket::get_timeout(){return _timeout;}

Socket::~Socket(){}
