#ifndef SOCKET_H
# define SOCKET_H

# include "webserver.h"
# include "request.hpp"
# include "parser.hpp"
# include "cgi.hpp"


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
		int	_timeout;
		Request _request;
		Parser	_parser;
		CGI _CGI;

	public:
		Socket();
		Socket(int domain, int type,
				int protocol, int port,
				int interface, int timeout);
		~Socket();

		int		get_domain();
		int		get_type();
		int		get_protocol();
		int		get_port();
		int		get_interface();
		int		get_fd();
		int		get_timeout();
		void		set_cgi(CGI &ref) {_CGI = ref;}
		void		set_request(Request & ref) {_request = ref;}
		Request		get_request(void) {return (_request);}
		void		set_parser(Parser & ref) {_parser = ref;}
		Parser		get_parser(void) {return (_parser);}
};

#endif
