/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aglorios <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/07 14:48:27 by aglorios          #+#    #+#             */
/*   Updated: 2021/06/08 16:54:34 by aglorios         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
#define PARSER_HPP

#include "webserver.h"
#include <algorithm>
# include <vector>
# include <iterator>
# include <list>
# include <stack>

class Parser
{
	private:
		std::string _conf_file;
		int			size_file;
		std::string	_index;
//		int	_queue;
		int	_listen_port;
		int _timeout;
		std::string	_host_address;
		std::string	_server_name;
		int _client_max_body_size;
	public:
		Parser(void) {}
		~Parser(void) {}
		bool copy_file(char *file);
		bool save_data(void);
		int int_val(std::string cmp, std::string cmd);
		bool client_body_size_check(std::string tab);
		std::string str_val(std::string cmp, std::string cmd);
		std::string *stotab(void);
};

#endif
