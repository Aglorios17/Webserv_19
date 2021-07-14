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

std::string strtrim_end(std::string &s, char c)
{
	size_t i = s.size() - 1;
	while (s[i] == c)
		i--;
	s.erase(i + 1, s.size());
	return s;
}

std::string strtrim(std::string &s, char c)
{
	s.erase(0, s.find_first_not_of(c));
	return s;
}
void msleep(int tms)
{
    struct timeval tv;
    tv.tv_sec  = tms / 1000;
    tv.tv_usec = (tms % 1000) * 1000;
    select (0, NULL, NULL, NULL, &tv);
}

void clean_substring(std::string &main, std::string to_delete)
{
	size_t pos = main.find(to_delete);	
	if (pos != std::string::npos)
		main.erase(pos, to_delete.length());
}

void clean_path(std::string &path)
{
	clean_substring(path, "/");
}

bool	file_exists(char const* name)
{

	std::ifstream f(name);
	return f.good();
}

std::string *str_add_back(std::string *tab, std::string add)
{
	std::string *ret = new std::string[tab->size() + 1];
	for (size_t i = 0; i < tab->size(); i++)
		ret[i] = tab[i];
	ret[ret->size()] = add;
	delete[] tab;
	return (ret);
}

int *int_in_tab(int *tab, int add, int size)
{
	int length = size - 1;
	int *ret = new int[size];
	for (int i = 0; i < length; i++)
		ret[i] = tab[i];
	ret[length] = add;
	delete[] tab;
	return (ret);
}

int	tab_len(int *tab)
{
	int ret = 0;
	for (int i = 0; tab[i]; i++)
		ret++;
	return (ret);
}

int *int_add_back(int *tab, int *add, int length, int size_add)
{
	int *ret = new int[length + size_add];
	int a = 0;
	for (int i = 0; i < (length + size_add); i++)
	{
		if (i < length)
			ret[i] = tab[i];
		else
			ret[i] = add[a++];
	}
	delete[] tab;
	delete[] add;
	return (ret);
}

std::string get_extension(std::string file)
{
	std::string type;
	std::size_t pos = file.find('.');

	if (pos == std::string::npos)
		return file;
	type = file.substr(pos+1);
	return type;
}

std::string get_time(t_data *data)
{
/*
	int sday = 86400;
	int shour = 3600;
	int smin = 60;
	struct timeval tv;
	struct timeval tz;
	std::string time = "Date: ";

	gettimeofday(&tv, &tz);
	long hms = tv.tv_sec % sday;
	hms += tz.tz_dsttime * shour;
  	hms -= tz.tz_minuteswest * smin;
  	hms = (hms + sday) % sday;
	time += std::to_string(hms / shour) + ":";
	time += std::to_string((hms % shour) / smin) + ":";
	time += std::to_string((hms % shour) % smin) + " GMT";
*/
	time_t now = time(0);
	char* dt = ctime(&now);
	std::string time = "Date : ";
	char *token = strtok(dt, " ");
	int i = 0;
	std::string day;
	std::string month;
	std::string year;
	std::string hour;
	std::string nbday;
	while (token != NULL)
	{
		if (i == 0)
			day = token;
		if (i == 1)
			month = token;
		if (i == 2)
			nbday = token;
		if (i == 3)
			hour = token;
		if (i == 4)
			for (int y = 0; token[y] != '\n'; y++)
				year += token[y];
		i++;
		token = strtok(NULL, " ");
	}
	std::string add = (day + ", " + nbday + " " + month + " " + year + " " + hour + " GMT\r\n");
	std::string tmp(data->last); /////////// lol c est pas tres fonctionnel
	if (add != tmp)
	{
		free(data->last);
		data->last = new char[add.length() + 1];
		strcpy(data->last, add.c_str());
	}
	time += add;
	return (time);
}

std::string bypass_tab(char *str)
{
	std::string string;
	for (int y = 0; str[y]; y++)
		if (str[y] != '\t')
			string += str[y];
	return (string);
}

int hexa_to_decimal(std::string value)
{
	unsigned int x;   
    std::stringstream ss;

    ss << std::hex << value;
    ss >> x;
    return (static_cast<int>(x));
}

int get_server(Socket sock [], int **holdr, int mode)
{
	static int *fds;
	static int nport;


	if (mode == SET)	
	{
		nport = sock[0].get_parser().get_nport();
		fds = (int*) malloc((nport + 1) * 4);
		memset(fds, 0, nport);
		for (int i = 0; i < nport ; i++)
			fds[i] = sock[i].get_fd();	
	}
	else if (mode == GET)
		for (int i = 0; i < nport ; i++)
			for (int i = 0; i < nport ; i++)
				*holdr = &fds[0];
	else
		free(fds);
	return nport;
}

int is_server(int fd)
{
	int *fds;
	int nport;
	nport = get_server(NULL, &fds, GET);

	for (int i = 0; i < nport; i++)
		if (fd == fds[i])
			return nport;
	return 0;
}
