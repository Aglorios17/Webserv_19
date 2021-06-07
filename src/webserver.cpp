#include "includes/webserver.h"

int main(int argc, char **argv)
{
	Socket			defaultSocket;
	Parser			parser;
	struct sockaddr_in	*address;
	
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
	address = new struct sockaddr_in;
	configure(defaultSocket, address);

	bind(defaultSocket.get_fd(),
			(struct sockaddr *)address,
			sizeof(struct sockaddr_in));

	run_server(defaultSocket, (struct sockaddr*)address);
	return (0);
}
