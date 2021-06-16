/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aglorios <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/07 14:48:27 by aglorios          #+#    #+#             */
/*   Updated: 2021/06/16 13:14:14 by aglorios         ###   ########.fr       */
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
		bool request_data(void);
		bool request_method_check(std::string line);
		void	add(char *buffer);
		std::string *stotab(void);
		std::string str_ret(std::string str, std::string cmd);
};

#endif
