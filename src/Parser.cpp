#include "includes/webServer.h"

/* It could be possible to implement 
 * more compatibilities see:
 * https://sites.ualberta.ca/dept/chemeng/AIX-43/share/man/info/C/a_doc_lib/aixprggd/progcomc/skt_types.htm
 */

bool	domain_is_valid(int domain)
{ return (domain == AF_INET || domain == AF_INET6 /*...*/); }

bool	type_is_valid(int type) 
{ return (type == SOCK_STREAM || type == SOCK_DGRAM /*...*/); }

bool	protocol_is_valid(int protocol)
{ return (protocol == PF_UNIX || protocol == PF_INET /*...*/); }

bool is_valid(int domain, int type, int protocol)
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
	}
	catch(std::string &error) { std::cout<<error<<std::endl; }

	return true;
}
