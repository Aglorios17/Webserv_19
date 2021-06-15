#ifndef SOCKET_H
#define SOCKET_H

#include "webserver.h"

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
		int	_port;
		int	_interface;
		int	_socket_fd;
		int _timeout;

	public:
		Socket();
		Socket(int domain, int type,
				int protocol, int port,
				int interface, int timeout);
		~Socket();

		int			get_domain();
		int			get_type();
		int			get_protocol();
		int			get_port();
		int			get_interface();
		int			get_fd();
		int			get_timeout();
};

#endif
