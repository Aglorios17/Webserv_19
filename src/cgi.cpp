/*
 * CGI:
 *
 * [SERVER_PROTOCOL]:	HTTP/1.1
 * [GATEWAY_INTERFACE]:	path/to/cgi
 * [REQUEST_METHOD]:	GET/POST/DELETE
 * [CONTENT_LENGTH]:	size of content in byte
 * [PATH_INFO]:		relative/path/to/file
 * [SERVER_PORT]:	PORT (list of ports ?)
 * [SERVER_SOFTWARE]:	webserver/1.0
 *
 * [GATEWAY_INTERFACE]: CGI/1.1
 * [SCRIPT_FILENAME]:	path/to/cgi
 * [PATH_TRANSLATED]:	full/path/to/file
 * [QUERY_STRING]:	what follow the ? in the requested url
 * [REMOTE_ADDR]:	real IP adress of client
 * [REMOTE_USER]:	name of user after authentification
 * [SERVER_NAME]:	Host name or ip adress of the computer running the webserver
 *
 * [REDIRECT_STATUS]:	200
 * [REMOTE_IDENT]:	? (not necessary? https://datatracker.ietf.org/doc/html/rfc3875#page-16)
 * [REMOTE_URI]:	?
 */

#include "../includes/webserver.h"

char *join(char *s1, char *s2)
{
	char *res = (char*)malloc(strlen(s1) + strlen(s2));

	if (!res)
		return res;
	res = strcpy(res, s1);
	res = strcat(res, s2);
	return res;
}

CGI::CGI()
{
	REQUEST_METHOD = "";
	PATH_INFO = "";
	SERVER_PORT = "";
	SERVER_SOFTWARE = "";
	GATEWAY_INTERFACE = "";
	SCRIPT_FILENAME = "";
	PATH_TRANSLATED = "";
	QUERY_STRING = "";
	REMOTE_ADDR = "";
	REMOTE_USER = "";
	SERVER_NAME = "";
	REDIRECT_STATUS = "";
}

void	CGI::set_cgi_env()
{
/*
 * Should get all variable from webserv
 * request class and parser class	
 *
 * This function should be called everytime a new request is set.
 */
	
}

char*	get_env(std::string s1, std::string s2)
{
	std::string res;

	res = s1 + s2;
	return (char*)res.c_str();
}
int	CGI::execute_cgi()
{
	char *env[14]; 

	env[0] = (char*)"SERVER_PROTOCOL=HTTP/1.1";
	env[1] = get_env("REQUEST_METHOD=", REQUEST_METHOD);
	env[2] = get_env("PATH_INFO=", PATH_INFO);
	env[3] = get_env("SERVER_PORT=", SERVER_PORT);
	env[4] = get_env("SERVER_SOFTWARE=", SERVER_SOFTWARE);
	env[5] = get_env("GATEWAY_INTERFACE=", GATEWAY_INTERFACE);
	env[6] = get_env("SCRIPT_FILENAME=", SCRIPT_FILENAME);
	env[7] = get_env("PATH_TRANSLATED=", PATH_TRANSLATED);
	env[8] = get_env("QUERY_STRING=", QUERY_STRING);
	env[9] = get_env("REMOTE_ADDR=", REMOTE_ADDR);
	env[10] = get_env("REMOTE_USER=", REMOTE_USER);
	env[11] = get_env("SERVER_NAME=", SERVER_NAME);
	env[12] = get_env("REDIRECT_STATUS=", REDIRECT_STATUS);
	env[13] = NULL;
	
	return (0);
}

void	CGI::set_PATH_INFO(std::string _PATH_INFO) { PATH_INFO = _PATH_INFO; }

void	CGI::set_SERVER_PORT(std::string _SERVER_PORT) { SERVER_PORT = _SERVER_PORT; }

void	CGI::set_SERVER_SOFTWARE(std::string _SERVER_SOFTWARE) { SERVER_SOFTWARE = _SERVER_SOFTWARE; }

void	CGI::set_GATEWAY_INTERFACE(std::string _GATEWAY_INTERFACE) { GATEWAY_INTERFACE = _GATEWAY_INTERFACE; }

void	CGI::set_SCRIPT_FILENAME(std::string _SCRIPT_FILENAME) { SCRIPT_FILENAME = _SCRIPT_FILENAME; }

void	CGI::set_PATH_TRANSLATED(std::string _PATH_TRANSLATED) { PATH_TRANSLATED = _PATH_TRANSLATED; }

void	CGI::set_QUERY_STRING(std::string _QUERY_STRING) { QUERY_STRING = _QUERY_STRING; }

void	CGI::set_REMOTE_ADDR(std::string _REMOTE_ADDR) { REMOTE_ADDR = _REMOTE_ADDR; }

void	CGI::set_REMOTE_USER(std::string _REMOTE_USER) { REMOTE_USER = _REMOTE_USER; }

void	CGI::set_SERVER_NAME(std::string _SERVER_NAME) { SERVER_NAME = _SERVER_NAME; }

void	CGI::set_REQUEST_METHOD(std::string _REQUEST_METHOD) { REQUEST_METHOD = _REQUEST_METHOD; }

void	CGI::set_REDIRECT_STATUS(std::string _REDIRECT_STATUS) { REDIRECT_STATUS = _REDIRECT_STATUS; }

CGI&	CGI::operator=(CGI& ref)
{
	if (this == &ref)
		return *this;

		PATH_INFO = ref.PATH_INFO;
		SERVER_PORT = ref.SERVER_PORT; 
		SERVER_SOFTWARE = ref.SERVER_SOFTWARE; 
		GATEWAY_INTERFACE = ref.GATEWAY_INTERFACE; 
		SCRIPT_FILENAME = ref.SCRIPT_FILENAME; 
		PATH_TRANSLATED = ref.PATH_TRANSLATED; 
		QUERY_STRING = ref.QUERY_STRING; 
		REMOTE_ADDR = ref.REMOTE_ADDR; 
		REMOTE_USER = ref.REMOTE_USER; 
		SERVER_NAME = ref.SERVER_NAME; 
		REDIRECT_STATUS = ref.REDIRECT_STATUS; 

		return *this;
}
