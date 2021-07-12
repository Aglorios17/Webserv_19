/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aglorios <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/07 14:48:27 by aglorios          #+#    #+#             */
/*   Updated: 2021/06/17 16:28:27 by aglorios         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "webserver.h"
#include <algorithm>
# include <vector>
# include <iterator>
# include <list>
# include <stack>

class Request
{
	private:
		std::string _method;
		std::string _arg_method;
		std::string _http_method;
		std::string _cgi;
		std::string _buffer;
		std::string _host;
		std::string _referer;
		std::string _connection;
		std::string _content_type;
		std::string	_content_length;
		std::string	_content_encoding;
		std::string	_user_agent;
		std::string	_transfer_encoding;
		std::string	_accept_encoding;
		std::string _body;
		int  _port;
		int			_size_buf;
		int			_status;
	public:
		Request(void) {}
		~Request(void) {}
		Request & operator=(Request &cpy)
		{
			if (this != &cpy)
			{
	 			_method = cpy._method;
	 			_arg_method = cpy._arg_method;
	 			_http_method = cpy._http_method;
	 			_host = cpy._host;
	 			_referer = cpy._referer;
				_connection = cpy._connection;
				_content_type = cpy._content_type;
				_content_length = cpy._content_length;
				_content_encoding = cpy._content_encoding;
				_status = cpy._status;
				_user_agent = cpy._user_agent;
				_transfer_encoding = cpy._transfer_encoding;
				_accept_encoding = cpy._accept_encoding;
				_cgi = cpy._cgi;
				_body = cpy._body;
				_port = cpy._port;
				_buffer = cpy._buffer;
			}
			return (*this);
		}
		void		init(void);
		bool		request_data(void);
		int			add_request_data(std::string tab);
		bool		request_method_check(std::string line);
		void		add(char *buffer);
		std::string	*stotab(void);
		std::string str_ret(std::string str, std::string cmd);
		int			int_ret(std::string str, std::string cmd);
		std::string	stock_body(std::string *tab, int y, int max);

		int		get_port(void) {return (_port);}
		std::string	get_host(void) {return (_host);}
		std::string	get_method(void) {return (_method);}
		std::string	get_arg_method(void) {return (_arg_method);}
		std::string	get_cgi(void) {return (_cgi);}
		std::string	get_http_method(void) {return (_http_method);}
		std::string	get_referer(void) {return (_referer);}
		std::string	get_connection(void) {return (_connection);}
		std::string	get_content_type(void) {return (_content_type);}
		std::string	get_body(void) {return (_body);}
		int		get_content_length(void)
		{
			if (_content_length != "")
				return (stoi(_content_length));
			else
				return (0);
		}
		std::string	get_content_encoding(void) {return (_content_encoding);}
		std::string	get_accept_encoding(void) {return (_accept_encoding);}
		std::string	get_transfer_encoding(void) {return (_transfer_encoding);}
		std::string	get_user_agent(void) {return (_user_agent);}
		std::string	get_buffer(void) {return (_buffer);}
		int			get_status(void) {return (_status);}
};

#endif
