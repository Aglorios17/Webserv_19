/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elajimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/07 16:09:09 by elajimi           #+#    #+#             */
/*   Updated: 2021/06/28 16:41:54 by aglorios         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/webserver.h"

int main(int argc, char **argv)
{
	Parser					parser;
	struct sockaddr_in*		address;
	POLLFD*				fds;
	struct poll			*poll;

	fds = NULL;

	
	if (argc != 2)
	{
		std::cout << "Need config file" << std::endl;
		return (0);
	}
	if (!parser.copy_file(argv[1]))
	{
		std::cout << "Error config file" << std::endl;
		return (0);
	}
	int ip = 0;		
	if (inet_addr(&parser.get_server_name()[0]))
		ip = AF_INET;		
	else
	{
		std::cout << "INVALID IP" << std::endl;
		return (0);
	}
	int *port = parser.get_port();
	int nport = parser.get_nport();

	Socket socket[nport];

	for (int i = 0; i < nport; i++)
	{
		socket[i].set_sock(ip, SOCK_STREAM, 0, port[i], INADDR_ANY, parser.get_timeout());	

		socket[i].set_parser(parser);
		address = new struct sockaddr_in;

		poll = (struct poll*)malloc(sizeof(struct poll));
		init_poll_struct(poll);

		add_fd_to_poll(
				poll,
				set_poll(
					socket[i].get_fd(),
					POLLIN,
					O_NOFLAG));
		configure(socket[i], address);

		bind(socket[i].get_fd(),
				(struct sockaddr *)address,
				sizeof(struct sockaddr_in));
		if (listen(socket[i].get_fd(), 1) < 0)
			exit (EXIT_FAILURE);
	}

	run_server(socket, (struct sockaddr*)address, poll);

	return (0);
}
