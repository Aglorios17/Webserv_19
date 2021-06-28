/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aglorios <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/07 14:48:27 by aglorios          #+#    #+#             */
/*   Updated: 2021/06/28 17:00:39 by aglorios         ###   ########.fr       */
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
		std::string	_error_page;
//		int	_queue;
		int	_listen_port;
		int _timeout;
		std::string	_host_address;
		std::string	_server_name;
		std::string _root;
		std::string _cgi_extension;
		std::string _cgi_path;
		int _client_max_body_size;
	public:
		Parser(void) {}
		~Parser(void) {}
		Parser & operator=(Parser &cpy)
		{
			if (this != &cpy)
			{
				_conf_file = cpy._conf_file;
				size_file = cpy.size_file;
				_index = cpy._index;
				_error_page = cpy._error_page;
				_listen_port = cpy._listen_port;
				_timeout = cpy._timeout;
				_host_address = cpy._host_address;
				_server_name = cpy._server_name;
				_root = cpy._root;
				_client_max_body_size = cpy._client_max_body_size;
				_cgi_path = cpy._cgi_path;
				_cgi_extension = cpy._cgi_extension;
			}
			return (*this);
		}
		bool copy_file(char *file);
		bool save_data(void);
		bool put_data(std::string tab, int cgi);
		bool server_parser(std::string *tab_conf, int size_file);
		bool location_parser(std::string *tab_conf, int start, int end);
		int int_val(std::string cmp, std::string cmd);
		bool client_body_size_check(std::string tab);
		std::string str_val(std::string cmp, std::string cmd);
		std::string *stotab(void);
		int	get_port(void) {return (_listen_port);}
		int	get_timeout(void) {return (_timeout);}
		std::string get_domain(void) {return (_host_address);}
		std::string get_server_name(void) {return (_server_name);}
		std::string get_index(void) {return (_index);}
		std::string get_error_page(void) {return (_error_page);}
		std::string get_root(void) {return (_root);}
		std::string get_cgi_path(void) {return (_cgi_path);}
		std::string get_cgi_extension(void) {return (_cgi_extension);}
};

#endif
