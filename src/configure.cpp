#include "includes/webserver.h"

void configure_adress(Socket &sock, struct sockaddr_in *addr)
{
	//memset((char*)addr, 0, sizeof(*addr));

	addr->sin_family = sock.get_domain();
	addr->sin_port = htons(sock.get_port());
	addr->sin_addr.s_addr = htonl(sock.get_interface());
}

void configure(Socket &sock, struct sockaddr_in *addr)
{
	int opt = 1;
	setsockopt(sock.get_fd(), SOL_SOCKET, SO_REUSEADDR, &opt, 4);
	configure_adress(sock, addr);
}
