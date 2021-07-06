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

	printf("INDEX QUERY: [%s]\n", name);
	fflush(stdout);

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

int *int_add_back(int *tab, int *add)
{
	int length = sizeof(&tab)/sizeof(tab[0]);
	int size_add = sizeof(&add)/sizeof(add[0]);
	int *ret = new int[length + size_add];
	int a = 0;
	for (int i = 0; i < length + size_add; i++)
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
