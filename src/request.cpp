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

#include "../includes/webserver.h"

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
	if (str.size())
		tab[i] += str.substr(0);
//	for (int i = 0; i < _size_buf; i++)
//		std::cout<<"["<<tab[i]<<"]"<<std::endl;
	return (tab);
}


void Request::add(char *buffer)
{
	_size_buf = 0;
	if (!buffer)
		return ;
	std::string add(buffer);
	_buffer = add;
	std::string str(buffer);

//	str[strlen(buffer)] = '\0';
	int i = 0;
	size_t pos = 0;
	std::string delimiter = "\n";
	while((pos = str.find(delimiter)) != std::string::npos)
	{
		_size_buf++;
		i++;
		str.erase(0, pos + 1);
	}
	if (str.size())
		++_size_buf;

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
	while (i < (int)str.size() && str[i] != ' ')
		add += str[i++];
	add += '\0';
	return (add);
}

int Request::int_ret(std::string str, std::string cmd)
{
	int i = 0;
	std::string cmp;
	while (str[i] && (str[i] == ' ' || str[i] == '\t'))
		i++;
	while (str[i] && str[i] != ' ')
		cmp += str[i++];
	if (cmp != cmd)
		return (-1);
	std::string add;
	while (str[i] && (str[i] == ' ' || str[i] == '\t'))
		i++;
	while (i < (int)str.size() && str[i] != ' ')
		add += str[i++];
	add += '\0';
	for (size_t y = 0; y < add.size(); y++)
		if (!isdigit(add[y]))
			return (-1);
	return (stoi(add));
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
		std::string tmp = _host.substr(_host.find(':') + 1);
		_port = stoi(tmp);
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
		for (size_t y = 0; y < (_content_length.size() - 1); y++)
			if (_content_length[y] && !isdigit(_content_length[y]))
				return (400);
	}
	else if (tab.find("Content-Type:") != std::string::npos)
	{
		if (_content_type != "")
			return (400);
		if ((_content_type = str_ret(tab, "Content-Type:")) == "")
			return (415);
	}
	else if (tab.find("Transfer-Encoding:") != std::string::npos)
	{
		if (_transfer_encoding != "")
			return (400);
		if ((_transfer_encoding = str_ret(tab, "Transfer-Encoding:")) == "")
			return (415);
	}
	else if (tab.find("Accept-Encoding:") != std::string::npos)
	{
		if (_accept_encoding != "")
			return (400);
		if ((_accept_encoding = str_ret(tab, "Accept-Encoding:")) == "")
			return (415);
	}
	else if (tab.find("User-Agent:") != std::string::npos)
	{
		if (_user_agent != "")
			return (400);
		if ((_user_agent = str_ret(tab, "User-Agent:")) == "")
			return (415);
	}
	else if (tab.find("Connection:") != std::string::npos)
	{
		if (_connection != "")
			return (400);
		if ((_connection = str_ret(tab, "Connection:")) == "")
			return (400);
	}
	else if (tab.find(":") == std::string::npos)
		return (1);
	return (200);
}

std::string Request::stock_body(std::string *tab, int y, int max)
{
	std::string body;

//	std::cout << "encoding : |" << _transfer_encoding << "|"<<std::endl;
	if (_transfer_encoding.find("chunked") == std::string::npos)
	{
		for (int i = y + 1; i < max; i++)
		{
			body += tab[i].substr(0);
			if (i + 1 < max)
				body += "\n";
		}
	}
	else
	{
//		std::cout << "chunked" <<std::endl;
		int size;
		for (int i = y + 1; i < max - 1; i++)
		{
			size = hexa_to_decimal(tab[i]);
			if (size == 0)
			{
				body += "\n";
				break ;	
			}
			i++;
			if (tab[i] == "")
				break ;
			int u = 0;
			for (int a = 0; a < size; a++)
			{
				int b = (int)tab[i].size();
				if (u > b)
				{
					u = 0;
					i++;
				}
				body += tab[i][u++];
			}
		}
	}
	return (body);
}

void Request::init(void)
{
	_status = 200;
	_method = "";
	_arg_method = "";
	_http_method = "";
	_cgi = "";
	_host = "";
	_port = 0;
	_referer = "";
	_connection = "";
	_content_type = "";
	_content_length = "";
	_content_encoding = "";
	_user_agent = "";
	_transfer_encoding = "";
	_accept_encoding = "";
}

bool Request::request_data(void)
{
	std::string *tab = stotab();
	int			error_method = 0;
	init();
	if (_buffer.find("no request") != std::string::npos)
	{
		std::cout << "NO REQUEST\n";
		return (1);
	}
	if (!tab)
	{
		_status = 204;
		return (0);
	}
	if (!request_method_check(tab[0]))
		error_method = _status;
	for (int y = 1; y < _size_buf - 1; y++)
	{
		if ((_status = add_request_data(tab[y])) != 200)
		{
			if (error_method)
			{
				_status = error_method;
				delete[] tab;
				return (0);
			}
			else if (_status == 1)
			{
				if (_method == "POST")
				{
					_body = stock_body(tab, y, _size_buf);
					_status = 200;
					delete[] tab;
					return (1);
				}
				else
				{
					_status = 400;
					delete[] tab;
					return (0);
				}
			}
			else
			{
				delete[] tab;
				return (0);
			}
		}
	}
	if (error_method)
	{
		_status = error_method;
		delete[] tab;
		return (0);
	}
	delete[] tab;
	return (1);
}
