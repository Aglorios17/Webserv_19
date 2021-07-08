/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aglorios <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/07 15:19:04 by aglorios          #+#    #+#             */
/*   Updated: 2021/06/28 16:59:01 by aglorios         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/webserver.h"

/* It could be possible to implement 
 * more compatibilities see:
 * https://sites.ualberta.ca/dept/chemeng/AIX-43/share/man/info/C/a_doc_lib/aixprggd/progcomc/skt_types.htm
 */
/////////////////////////////////////////////////////////////// Parser

int	end_line(int i, std::string cmp)
{
	int end = 0;
	while (cmp[i])
	{
		if (cmp[i] != ' ' && cmp[i] != '\t')
		{
			if (cmp[i] == ';' && end)
			{
				if (end)
					return (0);
				else
					end++;
			}
		}
		i++;
	}
	return (1);
}

int *Parser::int_tab_val(std::string cmp, std::string cmd)
{
	char *token = strtok(&cmp[0], " ");
	int i = 0;
	bool end = 0;
	int	*tab = NULL;
	while (token != NULL)
	{
		if (end)
			return (NULL);
		std::string tok = bypass_tab(token);
		if (tok == ";")
			end = 1;
		else if (i == 0 && tok != cmd)
			return (0);
		else if (i != 0)
		{
			if (tok.find(";") != std::string::npos)
			{
				for (size_t y = 0; y < tok.size(); y++)
					if (!isdigit(tok[y]) && (tok[y] != ';' && !tok[y + 1]))
						return (NULL);
				end = 1;
			}
			else
				for (size_t y = 0; y < tok.size(); y++)
					if (!isdigit(tok[y]))
						return (NULL);
			if (tab == NULL)
			{
				tab = int_in_tab(NULL, std::stoi(tok), 1);
				_tab_size++;
			}
			else
			{
				tab = int_in_tab(tab, std::stoi(tok), i);
				_tab_size++;
			}
		}
		i++;
		token = strtok(NULL, " ");
	}
	return (tab);
}

int Parser::int_val(std::string cmp, std::string cmd)
{
	int i = 0;
	while (cmp[i] && (cmp[i] == ' ' || cmp[i] == '\t'))
		i++;
	if (strncmp(&cmp[i], &cmd[0], cmd.length()))
		return (0);
	i += cmd.length();
	while (cmp[i] && (cmp[i] == ' ' || cmp[i] == '\t'))
		i++;
	int add = atoi(&cmp[i]);
	if (add <= 0)
		return (0);
	while (isdigit(cmp[i]))
		i++;
	if (!end_line(i, cmp))
		return (0);
	return (add);
}

std::string Parser::str_val(std::string cmp, std::string cmd)
{
	int i = 0;
	while (cmp[i] && (cmp[i] == ' ' || cmp[i] == '\t'))
		i++;
	if (strncmp(&cmp[i], &cmd[0], cmd.length()))
		return (0);
	i += cmd.length();
	while (cmp[i] && (cmp[i] == ' ' || cmp[i] == '\t'))
		i++;
	std::string add;
	while (cmp[i]  && cmp[i] != ';' && cmp[i] != ' ' && cmp[i] !='\t')
		add += cmp[i++];
	if (!end_line(i, cmp))
		return (0);
	return (add);
}

std::string *Parser::stotab(void)
{
	std::string *tab_conf = new std::string[size_file];
	int	put_int_tab = 0;
	for (int i = 0; _conf_file[i]; i++)
	{
		if (_conf_file[i] == '\n')
		{
			put_int_tab++;
			if (_conf_file[i + 1])
				i++;
			else
				break;	
		}
		tab_conf[put_int_tab] += _conf_file[i];
	}
	return (tab_conf);
}

bool server_norme(std::string *tab, int size)
{
	std::string check; 
	check = tab[size - 1];
	int i = 0;
	int end = 0;
	while (check[i] && (check[i] == ' ' ||check[i] == '\t'))
		i++;
	while (check[i] && check[i] == '}')
	{
		if (check[i++] == '}')
			end++;
	}
	while (check[i] && (check[i] == ' ' ||check[i] == '\t'))
		i++;
	if (end != 1 || check[i] != '\0')
		return (0);
	check = tab[0];
	i = 0;
	end = 0;
	if (check.find("server") != std::string::npos)
	{
		while (check[i] && (check[i] == ' ' ||check[i] == '\t'))
			i++;
		if (strncmp(&check[i], "server", 6))
			return (0);
		i += 6;
		while (check[i] && (check[i] == ' ' ||check[i] == '\t'))
			i++;
		while (check[i] && check[i] == '{')
		{
			if (check[i++] == '{')
				end++;
		}
		while (check[i] && (check[i] == ' ' ||check[i] == '\t'))
			i++;
		if (end != 1 || check[i] != '\0')
			return (0);
	}
	else
		return (0);
	return (1);
}

bool Parser::client_body_size_check(std::string tab)
{
	std::string add;
	if (_client_max_body_size != 0 || (add = str_val(tab, "client_max_body_size")) == "")
		return (0);
	if (!isdigit(add[0]))
		return (0);
	int i = 0;
	while (add[i])
	{
		if (!isdigit(add[i]))
		{
			if (add[i] == 'M' && !add[i + 1])
			{	
				if (!(_client_max_body_size = atoi(&add[0])))
					return (0);
			}
			else
				return (0);
		}
		i++;
	}
	return (1);
}

bool Parser::put_data(std::string tab, int cgi)
{
	if (tab.find("listen") != std::string::npos)
	{
		std::cout << std::endl;
		if (_listen_port != NULL)
		{
			int *port_tab = 0;
			int save_port = _tab_size;
			if (!(port_tab = int_tab_val(tab, "listen")))
				return (0);
			_add_size = _tab_size - save_port;
			_listen_port = int_add_back(_listen_port, port_tab, save_port, _add_size);
//			delete[] port_tab;
		}
		else
		{
			if (!(_listen_port = int_tab_val(tab, "listen")))
				return (0);
		}
	}
	else if (tab.find("timeout") != std::string::npos)
	{	
		if (_timeout != 0 || !(_timeout = int_val(tab, "timeout")))
			return (0);
	}
	else if (tab.find("server_name") != std::string::npos)
	{	
		if (_server_name != "" || (_server_name = str_val(tab, "server_name")) == "")
			return (0);
	}
	else if (tab.find("client_max_body_size") != std::string::npos)
	{
		if (!client_body_size_check(tab))
			return (0);
	}
	else if (tab.find("root") != std::string::npos)
	{	
		if (cgi == 0)
		{
			if (_root != "" || (_root = str_val(tab, "root")) == "")
				return (0);
		}
		else
			if (_cgi_path != "" || (_cgi_path = str_val(tab, "root")) == "")
				return (0);
	}
	else if (tab.find("index") != std::string::npos)
	{	
		if (_index != "" || (_index = str_val(tab, "index")) == "")
			return (0);
	}
	else if (tab.find("error_page") != std::string::npos)
	{	
		if (_error_page != "" || (_error_page = str_val(tab, "error_page")) == "")
			return (0);
	}
	return (1);
}

bool location_end(std::string *tab_conf, int end)
{
	char *token = strtok(&tab_conf[end][0], " ");
	int i = 0;
	while (token != NULL)
	{
		std::string tok = bypass_tab(token);
		if ((i == 0 && tok != "}") || i == 1)
			return (0);
		i++;
		token = strtok(NULL, " ");
	}
	return (1);
}

std::string ret_extension(std::string ext)
{
	std::string ret;
	char *token = strtok(&ext[0], ".");
	int i = 0;
	while (token != NULL)
	{
		if (i == 1)
			ret = bypass_tab(token);
		if (i == 2)
			return (0);
		i++;
		token = strtok(NULL, " ");
	}
	return (ret);
}

bool Parser::location_parser(std::string *tab_conf, int start, int end)
{
	int cgi = 0;
	char *token = strtok(&tab_conf[start][0], " ");
	std::string tok;
	int i = 0;
	while (token != NULL)
	{
		tok = bypass_tab(token);
		if ((i == 0 && tok != "location") || (i == 2 && tok != "{") || i == 3)
			return (0);
		if (i == 1)
		{
			if (_cgi_extension == "" && tok.find(".") != std::string::npos)
			{
				_cgi_extension = ret_extension(tok);
				cgi = 1;
			}
		}
		i++;
		token = strtok(NULL, " ");
	}
	if (!location_end(tab_conf, end))
		return (0);
	for (int x = start + 1; x < end; x++)
		if (!put_data(tab_conf[x], cgi))
			return (0);
	return (1);
}

bool Parser::server_parser(std::string *tab_conf, int size_file)
{
	for (int y = 1; y < size_file - 1; y++)
	{
		if (tab_conf[y].find("location") != std::string::npos)
		{
			int start = y;
			while (y < size_file && tab_conf[y].find("}") == std::string::npos)
				y++;
			if (!location_parser(tab_conf, start, y))
				return (0);
		}
		if (!put_data(tab_conf[y], 0))
			return (0);
	}
	return (1);
}

bool Parser::save_data(void)
{
	std::string *tab_conf = stotab();
	_listen_port = 0;
	_timeout = 0;
	_client_max_body_size = 0;
	_tab_size = 0;
	_add_size = 0;
	if (!server_norme(tab_conf, size_file))
		return (0);
	if (!server_parser(tab_conf, size_file))
		return (0);
	std::cout << "PORT : ";
	for (int i = 0; i < _tab_size ; i++)
	{
		std::cout << _listen_port[i] << " ";
		_nport += 1;
	}
	std::cout << std::endl;
	std::cout << "NPORT : " << _nport << std::endl;
	std::cout << "Timeout : " << _timeout << std::endl;
	std::cout << "Server_name : " << _server_name << std::endl;
	std::cout << "client_max_body_size : "<< _client_max_body_size << std::endl;
	std::cout << "root : " << _root << std::endl;
	std::cout << "index : " << _index << std::endl;
	std::cout << "error_page : " << _error_page << std::endl;
	std::cout << "cgi_extension : " << _cgi_extension << std::endl;
	std::cout << "cgi_path : " << _cgi_path << std::endl;
	delete[] tab_conf;
	return (1);
}

bool Parser::copy_file(char *file)
{
	std::ifstream copy(file);
	std::string line;
	size_file = 0;

	if (!copy.is_open())
		return (0);
	while (std::getline(copy, line))
	{
		int i = 0;
		while (line[i] && (line[i] == ' ' || line[i] == '\t')) 
			i++;
		if (line[i] != '#' && line[i] != '\0')
		{
			for (int a = 0;line[a] && line[a] != '#'; a++)
			{
				if (line[a] == '\t')
					_conf_file += ' ';
				else
					_conf_file += line[a];
			}
			_conf_file += "\n";
			size_file++;
		}
	}
	copy.close();
//	std::cout << size_file << std::endl;
//	std::cout << _conf_file;
	if (!save_data())
		return (0);
	return (1);
}

////////////////////////////////////////////////////////////////

bool	domain_is_valid(int domain)
{
	return (
		domain == AF_INET ||
		domain == AF_INET6
		/*...*/
		);
}

bool	type_is_valid(int type) 
{
	return (
		type == SOCK_STREAM ||
		type == SOCK_DGRAM
		/*...*/
		);
}

bool	protocol_is_valid(int protocol)
{
	return (
		protocol == PF_UNIX ||
		protocol == PF_INET ||
		protocol == 0
		/*...*/
		);
}

bool	port_is_valid(int port)
{
	/*
	 * For now, no port checker has been implemented
	 * for neither TCP or UDP.
	 * https://en.wikipedia.org/wiki/List_of_TCP_and_UDP_port_numbers
	 * This can be done by applying a bind function with the
	 * desired port.
	 */

	(void)port;
	return (1);
	//	port == 8080 ||
	//	port == 8000 
	//	/*...*/
	//	);
}

bool is_valid(int domain, int type, int protocol, int port)
{
	/*
	 * Not sure how to properly implement this try/catch branching
	 * This function is doing more than it should
	 */
	try
	{
		if (domain_is_valid(domain) == false)
			throw "Domain is not valid";
		else if (type_is_valid(type) == false)
			throw "Type is not valid";
		else if (protocol_is_valid(protocol) == false)
			throw "Protocol is not valid";
		else if (port_is_valid(port) == false)
			throw "port is not valid";
	}
	catch(char const *error) { std::cout<<error<<std::endl; }
	return true;
}
