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

int *int_add_back(int *tab, int add)
{
	int length = sizeof(&tab)/sizeof(tab[0]);
	int *ret = new int[length + 1];
	for (int i = 0; i < length; i++)
		ret[i] = tab[i];
	ret[length + 1] = add;
	delete[] tab;
	return (ret);
}
