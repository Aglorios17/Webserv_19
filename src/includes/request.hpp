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
		std::string _buffer;
		std::string _host;
		std::string _referer;
		std::string	_connection;
		int			_size_buf;
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
			}
			return (*this);
		}
		bool request_data(void);
		bool request_method_check(std::string line);
		void	add(char *buffer);
		std::string*	stotab();
		std::string	str_ret(std::string str, std::string cmd);
		std::string	get_host(void) {return (_host);}
		std::string	get_method(void) {return (_method);}
		std::string	get_arg_method(void) {return (_arg_method);}
		std::string	get_http_method(void) {return (_http_method);}
		std::string	get_referer(void) {return (_referer);}
		std::string	get_connection(void) {return (_connection);}
};

#endif
