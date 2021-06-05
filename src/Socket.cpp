#include "includes/webServer.h"

Socket::Socket() :
       	_domain(AF_INET), _type(SOCK_STREAM), _protocol(0)
{
	is_valid(_domain, _type, _protocol);
	_socket_fd = socket(_domain, _type, _protocol);
}

Socket::Socket(int domain, int type, int protocol) : 
	_domain(domain), _type(type), _protocol(protocol)
{
	is_valid(_domain, _type, _protocol);
	_socket_fd = socket(_domain, _type, _protocol);
}

int Socket::get_domain(){return _domain;}

int Socket::get_type(){return _type;}

int Socket::get_protocol(){return _protocol;}

int Socket::get_fd(){return _socket_fd;}

Socket::~Socket(){}
