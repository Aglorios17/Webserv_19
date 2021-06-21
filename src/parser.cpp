/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aglorios <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/07 15:19:04 by aglorios          #+#    #+#             */
/*   Updated: 2021/06/08 16:54:32 by aglorios         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/webserver.h"

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
bool Parser::save_data(void)
{
	std::string *tab_conf = stotab();
	_listen_port = 0;
	_timeout = 0;
	_client_max_body_size = 0;
	if (!server_norme(tab_conf, size_file))
		return (0);
	for (int y = 1; y < size_file - 1; y++)
	{
		if (tab_conf[y].find("listen") != std::string::npos)
		{	
			if (_listen_port != 0 || !(_listen_port = int_val(tab_conf[y], "listen")))
				return (0);
		}
		else if (tab_conf[y].find("timeout") != std::string::npos)
		{	
			if (_timeout != 0 || !(_timeout = int_val(tab_conf[y], "timeout")))
				return (0);
		}
		else if (tab_conf[y].find("server_name") != std::string::npos)
		{	
			if (_server_name != "" || (_server_name = str_val(tab_conf[y], "server_name")) == "")
				return (0);
		}
		else if (tab_conf[y].find("client_max_body_size") != std::string::npos)
		{
			if (!client_body_size_check(tab_conf[y]))
				return (0);
		}
		else if (tab_conf[y].find("index") != std::string::npos)
		{	
			if (_index != "" || (_index = str_val(tab_conf[y], "index")) == "")
				return (0);
		}
	}
	std::cout << "PORT : "<< _listen_port << std::endl;
	std::cout << "Timeout : "<< _timeout << std::endl;
	std::cout << "Server_name : "<< _server_name << std::endl;
	std::cout << "client_max_body_size : "<< _client_max_body_size << std::endl;
	std::cout << "index : "<< _index << std::endl;
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
				_conf_file += line[a];
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

	return (
		port == 8080 ||
		port == 8000 
		/*...*/
		);
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
