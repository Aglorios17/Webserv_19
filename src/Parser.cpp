#include "includes/webserver.h"

/* It could be possible to implement 
 * more compatibilities see:
 * https://sites.ualberta.ca/dept/chemeng/AIX-43/share/man/info/C/a_doc_lib/aixprggd/progcomc/skt_types.htm
 */

bool	domain_is_valid(int domain)
{
	return (
		domain == AF_INET ||
		domain == AF_INET6
		/*...*/
		);
}

bool	type_is_valid(int type) 
{
	return (
		type == SOCK_STREAM ||
		type == SOCK_DGRAM
		/*...*/
		);
}

bool	protocol_is_valid(int protocol)
{
	return (
		protocol == PF_UNIX ||
		protocol == PF_INET ||
		protocol == 0
		/*...*/
		);
}

bool	port_is_valid(int port)
{
	/*
	 * For now, no port checker has been implemented
	 * for neither TCP or UDP.
	 * https://en.wikipedia.org/wiki/List_of_TCP_and_UDP_port_numbers
	 * This can be done by applying a bind function with the
	 * desired port.
	 */

	return (
		port == 8080 ||
		port == 8000 
		/*...*/
		);
}

bool is_valid(int domain, int type, int protocol, int port)
{
	/*
	 * Not sure how to properly implement this try/catch branching
	 * This function is doing more than it should
	 */
	try
	{
		if (domain_is_valid(domain) == false)
			throw "Domain is not valid";
		else if (type_is_valid(type) == false)
			throw "Type is not valid";
		else if (protocol_is_valid(protocol) == false)
			throw "Protocol is not valid";
		else if (port_is_valid(port) == false)
			throw "port is not valid";
	}
	catch(char const *error) { std::cout<<error<<std::endl; }
	return true;
}
