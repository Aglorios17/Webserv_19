#include "../includes/webserver.h"

void print_reponse(std::string str)
{
	std::cout << YELLOW << "============ REPONSE ============" << RESET << std::endl;
	std::cout << YELLOW << str << RESET << std::endl;
	std::cout << YELLOW << "================================="<< RESET << std::endl;
}

void print_request(std::string str)
{
	std::cout << GREEN << "============ REQUEST ============" << RESET << std::endl;
	std::cout << GREEN << str << RESET << std::endl;
	std::cout << GREEN << "================================="<< RESET << std::endl;
}
