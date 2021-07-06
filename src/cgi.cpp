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

void	CGI::set_var(Request& request, Parser& parser)
{
/*
 * Should get all variable from webserv
 * request class and parser class	
 *
 * This function should be called everytime a new request is set.
 */
	char cwd[256];

	getcwd(cwd, 256);
	std::string full_path(cwd);
	full_path += "/www";

	REQUEST_METHOD = request.get_method();
	PATH_INFO = request.get_arg_method();
	SERVER_PORT = parser.get_port()[0];
	SERVER_SOFTWARE = "webserver/1.0";
	GATEWAY_INTERFACE = "CGI/1.1";
	SCRIPT_FILENAME = parser.get_cgi_path();
	PATH_TRANSLATED = (char*)full_path.c_str();
	std::string referer = request.get_referer();
	if (referer.find('?') != std::string::npos)
		QUERY_STRING = referer.substr(referer.find('?') + 1);
	SERVER_NAME = "http://localhost";
	REDIRECT_STATUS =  "200";
//------------------------------------
	std::cout<<"REQUEST_METHOD:["<<REQUEST_METHOD<<"]"<<std::endl;
	std::cout<<"PATH_INFO:	["<<PATH_INFO<<"]"<<std::endl;
	std::cout<<"SERVER_PORT	["<<SERVER_PORT<<"]"<<std::endl;
	std::cout<<"GATEWAY_INTERFACE["<<GATEWAY_INTERFACE<<"]"<<std::endl;
	std::cout<<"SCRIPT_FILENAME["<<SCRIPT_FILENAME<<"]"<<std::endl;
	std::cout<<"PATH_TRANSLATED["<<PATH_TRANSLATED<<"]"<<std::endl;
	std::cout<<"QUERY_STRING ["<<QUERY_STRING<<"]"<<std::endl;
	std::cout<<"SERVER_NAME	["<<SERVER_NAME<<"]"<<std::endl;
	std::cout<<"REDIRECT_STATUS ["<<REDIRECT_STATUS<<"]"<<std::endl;
	//REMOTE_ADDR = 
	//REMOTE_USER = 
//------------------------------------
}

void CGI::init_var()
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

CGI::CGI() { init_var(); }

CGI::CGI(Request request, Parser parser)
{
	init_var();
	set_var(request, parser);
	set_env(&env[0]);
}

char*	strcat(std::string s1, std::string s2)
{
	return (strdup((char*)(s1 + s2).c_str()));
}

void CGI::set_env(char **_env)
{
	_env[0] = (char*)"SERVER_PROTOCOL=HTTP/1.1";
	_env[1] = strcat("REQUEST_METHOD=", REQUEST_METHOD);
	_env[2] = strcat("PATH_INFO=", PATH_INFO);
	_env[3] = strcat("SERVER_PORT=", SERVER_PORT);
	_env[4] = strcat("SERVER_SOFTWARE=", SERVER_SOFTWARE);
	_env[5] = strcat("GATEWAY_INTERFACE=", GATEWAY_INTERFACE);
	_env[6] = strcat("SCRIPT_FILENAME=", SCRIPT_FILENAME);
	_env[7] = strcat("PATH_TRANSLATED=", PATH_TRANSLATED);
	_env[8] = strcat("QUERY_STRING=", QUERY_STRING);
	_env[9] = strcat("REMOTE_ADDR=", REMOTE_ADDR);
	_env[10] =strcat("REMOTE_USER=", REMOTE_USER);
	_env[11] =strcat("SERVER_NAME=", SERVER_NAME);
	_env[12] =strcat("REDIRECT_STATUS=", REDIRECT_STATUS);
	_env[13] = NULL;
}

int	CGI::execute_cgi() { return (0); }

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
		ref.set_env(&env[0]);

		return *this;
}

void	ft_freetab(char **tabs)
{
	int a;

	a = 0;
	while (tabs[a])
	{
		free(tabs[a]);
		tabs[a] = NULL;
		a++;
	}
	tabs = NULL;
}

CGI::~CGI(void)
{
	//ft_freetab(env);
}
