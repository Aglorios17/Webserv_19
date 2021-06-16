/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aglorios <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/07 15:19:04 by aglorios          #+#    #+#             */
/*   Updated: 2021/06/16 13:14:16 by aglorios         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/webserver.h"

std::string *Request::stotab(void)
{
	std::string *tab = new std::string[_size_buf];
	int	put_int_tab = 0;
	for (int i = 0; _buffer[i]; i++)
	{
		if (_buffer[i] == '\n')
		{
			put_int_tab++;
			if (_buffer[i + 1])
				i++;
			else
				break;
		}
		tab[put_int_tab] += _buffer[i];
	}
	return (tab);
}


void Request::add(char *buffer)
{
	_size_buf = 0;
	std::string add(buffer);
	_buffer = add;
	for (int i = 0; _buffer[i] ; i++)
		if (_buffer[i] == '\n')
			_size_buf++;
}

std::string Request::str_ret(std::string str, std::string cmd)
{
	int i = 0;
	std::string cmp;
	while (str[i] && (str[i] == ' ' || str[i] == '\t'))
		i++;
	while (str[i] && str[i] != ' ')
		cmp += str[i++];
	if (cmp != cmd)
		return (0);
	std::string add;
	while (str[i] && (str[i] == ' ' || str[i] == '\t'))
		i++;
	while (str[i] && str[i] != ' ')
		add += str[i++];
	return (add);
}

bool Request::request_method_check(std::string line)
{
	int i = 0;
	while (line[i] == ' ' || line[i] == '\t')
		i++;
	while (line[i] != ' ' || line[i] != '\t')
		_method += line[i++];
	while (line[i] == ' ' || line[i] == '\t')
		i++;
	while (line[i] != ' ' || line[i] != '\t')
		_arg_method += line[i++];
	while (line[i] == ' ' || line[i] == '\t')
		i++;
	while (line[i] != ' ' || line[i] != '\t')
		_http_method += line[i++];
	while (line[i] == ' ' || line[i] == '\t')
		i++;
	if (line[i] != '\0')
		return (0);
	if (strncmp(&_method[0], "GET\0", 4) && strncmp(&_method[0], "POST\0", 5) && strncmp(&_method[0], "DELETE\0", 7))
		return (0); 
	if (strncmp(&_http_method[0], "HTTP/1.1\0", 9))
		return (0); 
	return (1);
}

bool Request::request_data(void)
{
	std::string *tab = stotab();
	if (!request_method_check(tab[0]))
		return (0);
	for (int y = 1; y < _size_buf; y++)
	{
		if (tab[y].find("Host:") != std::string::npos)
		{	
			if (_host != "" || (_host = str_ret(tab[y], "Host:")) == "")
				return (0);
		}
		else if (tab[y].find("Referer:") != std::string::npos)
		{	
			if (_referer != "" || (_referer = str_ret(tab[y], "Referer:")) == "")
				return (0);
		}
		else if (tab[y].find("Connection:") != std::string::npos)
			if (_connection != "" || (_connection = str_ret(tab[y], "Connection:")) == "")
				return (0);
	}
	std::cout << "_host : "<< _host << std::endl;
	std::cout << "_referer : "<< _referer << std::endl;
	std::cout << "_connection : "<< _connection << std::endl;
	delete[] tab;
	return (1);
}
