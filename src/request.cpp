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

std::string* Request::stotab()
{
	std::string*	res = new std::string[19];
	std::string	str = _buffer.substr(0, _buffer.length());
	std::string	delimiter = "\n";
	size_t		pos = 0;
	int		i = 0;

	if (_buffer == "")
		return NULL;
	while ((pos = _buffer.find(delimiter)) != std::string::npos)
	{
		std::string tmp = _buffer.substr(0, pos);
		res[i] = _buffer.substr(0, pos);	
		res[i][pos-1] = '\0';
		i++;
		_buffer.erase(0, pos + delimiter.length());
	}
	return res;
}


void Request::add(char *buffer)
{
	_size_buf = 0;
	if (!buffer)
		return ;
	std::string tmp(buffer);
	_buffer = tmp;//add;

	for (int i = 0; (i = _buffer.find('\n', i))
			!= (int)std::string::npos; i++)
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
	fflush(stdout);
	//std::string add = new std::string;
	char add[1000];
	fflush(stdout);
	while (str[i] && (str[i] == ' ' || str[i] == '\t'))
		i++;
	int j = 0;
	while (str[i] && str[i] != ' ')
		add[j++] = str[i++];
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
	//if (!strncmp(&_http_method[0], "HTTP/1.1", strlen(&_http_method[0])))
		//return (0);/*Alessio: condition isnt valid anymore,
		//needs some fixing
	return (1);
}

bool Request::request_data(void)
{
	std::string *tab;
       	tab = stotab();
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
