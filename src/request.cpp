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
	std::string	*tab = new std::string[_size_buf + 1];
	std::string	str = _buffer.substr(0, _buffer.length());
	std::string	delimiter = "\n";
	size_t		pos = 0;
	int		i = 0;

	if (_buffer == "")
		return (NULL);
	while((pos = str.find(delimiter)) != std::string::npos)
	{
		tab[i] = str.substr(0, pos);
		tab[i++][pos - 1] = '\0';
		str.erase(0, pos + delimiter.length());
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
		{
			_arg_method = token;
			if (_arg_method.find(".") != std::string::npos)
				_cgi = _arg_method.substr(_arg_method.find(".") + 1);
		}
		if (i == 2)
			_http_method = token;
		if (i == 3)
		{
			_status = 400;
			return (0);
		}
		i++;
		token = strtok(NULL, " ");
	}
	if (strncmp(&_method[0], "GET", strlen(&_method[0])) && strncmp(&_method[0], "POST", strlen(&_method[0]))
			&& strncmp(&_method[0], "DELETE", strlen(&_method[0])))
	{
		_status = 501;
		return (0);
	}
	if (strncmp(&_http_method[0], "HTTP/1.1", strlen(&_http_method[0])))
	{
		_status = 505;
		return (0);
	}
	return (1);
}

int Request::add_request_data(std::string tab)
{
	if (tab.find("Host:") != std::string::npos)
	{
		if (_host != "")
			return (400);
		if ((_host = str_ret(tab, "Host:")) == "")
			return (400);
	}
	else if (tab.find("Referer:") != std::string::npos)
	{
		if (_referer != "")
			return (400);
		if ((_referer = str_ret(tab, "Referer:")) == "")
			return (400);
	}
	else if (tab.find("Content-Encoding:") != std::string::npos)
	{
		if (_content_encoding != "")
			return (400);
		if ((_content_encoding = str_ret(tab, "Content-Encoding:")) == "")
			return (400);
	}
	else if (tab.find("Content-Length:") != std::string::npos)
	{
		if (_content_length != "")
			return (400);
		if ((_content_length = str_ret(tab, "Content-Length:")) == "")
			return (411);
	}
	else if (tab.find("Content-Type:") != std::string::npos)
	{
		if (_content_type != "")
			return (400);
		if ((_content_type = str_ret(tab, "Content-Type:")) == "")
			return (415);
	}
	else if (tab.find("Connection:") != std::string::npos)
	{
		if (_connection != "")
			return (400);
		if ((_connection = str_ret(tab, "Connection:")) == "")
			return (400);
	}
	return (200);
}

bool Request::request_data(void)
{
	std::string *tab = stotab();
	_status = 200;
	if (!tab)
	{
		_status = 204;
		std::cout << "STATUS CODE : ||" << _status << "||\n";
		return (0);
	}
	if (!request_method_check(tab[0]))
	{
		std::cout << "STATUS CODE : ||" << _status << "||\n";
		return (0);
	}
	for (int y = 1; y < _size_buf; y++)
	{
		if ((_status = add_request_data(tab[y])) != 200)
		{
			std::cout << "STATUS CODE : ||" << _status << "||\n";
			return (0);
		}
	}
	std::cout << "STATUS CODE : ||" << _status << "||\n";
	delete[] tab;
	return (1);
}
