/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aglorios <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/07 15:19:04 by aglorios          #+#    #+#             */
/*   Updated: 2021/06/08 16:54:32 by aglorios         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/webserver.h"

/////////////////////////////////////////////////////////////// request Request

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

bool Request::request_data(void)
{
	std::string *tab = stotab();
	for (int y = 0; y < _size_buf; y++)
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
		{	
			if (_connection != "" || (_connection = str_ret(tab[y], "Connection:")) == "")
				return (0);
		}
	}
	std::cout << "_host : "<< _host << std::endl;
	std::cout << "_referer : "<< _referer << std::endl;
	std::cout << "_connection : "<< _connection << std::endl;
	delete[] tab;
	return (1);
}

////////////////////////////////////////////////////////////////
