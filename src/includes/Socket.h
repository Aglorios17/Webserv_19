#ifndef SOCKET_H
#define SOCKET_H

#include "webServer.h"

class Socket
{
	/*
	 * Compiles all variables necessary
	 * for the creation of a socket.
	 */
	private:
		int	_domain;
		int	_type;
		int	_protocol;
		int	_socket_fd;
	public:
		Socket();
		Socket(int domain, int type, int protocol);
		~Socket();

		int get_domain();
		int get_type();
		int get_protocol();
		int get_fd();
};

#endif
