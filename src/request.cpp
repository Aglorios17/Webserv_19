/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aglorios <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/07 15:19:04 by aglorios          #+#    #+#             */
/*   Updated: 2021/06/17 16:21:42 by aglorios         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/webserver.h"

std::string *Request::stotab(void)
{
	int	put_int_tab = 0;

	if (_buffer == "")
		return (NULL);
	std::string *tab = new std::string[_size_buf + 1];
	for (int i = 0; i < (int)_buffer.length(); i++)
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
	if (!buffer)
		return ;
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
	add += '\0';
	return (add);
}

bool Request::request_method_check(std::string line)
{
	char *token = strtok(&line[0], " ");
	int i = 0;
	while (token != NULL)
	{
		if (i == 0)
			_method = token;
		if (i == 1)
			_arg_method = token;
		if (i == 2)
			_http_method = token;
		if (i == 3)
			return (0);
		i++;
		token = strtok(NULL, " ");
	}
	if (strncmp(&_method[0], "GET", strlen(&_method[0])) && strncmp(&_method[0], "POST", strlen(&_method[0]))
			&& strncmp(&_method[0], "DELETE", strlen(&_method[0])))
		return (0);
	if (!strncmp(&_http_method[0], "HTTP/1.1", strlen(&_http_method[0])))
		return (0);
	return (1);
}

bool Request::request_data(void)
{
	std::string *tab = stotab();
	if (!tab)
	{
		std::cout << "BUFFER NULL" << std::endl;
		return (0);
	}
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
	delete[] tab;
	return (1);
}
