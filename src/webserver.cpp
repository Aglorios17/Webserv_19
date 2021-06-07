#include "includes/webserver.h"

int main()
{
	Socket			defaultSocket;
	struct sockaddr_in	*address;
	
	address = new struct sockaddr_in;
	configure(defaultSocket, address);

	bind(defaultSocket.get_fd(),
			(struct sockaddr *)address,
			sizeof(struct sockaddr_in));

	run_server(defaultSocket, (struct sockaddr*)address);
	return (0);
}
