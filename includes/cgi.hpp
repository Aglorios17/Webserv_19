#ifndef CGI_HPP
# define CGI_HPP

# include "./webserver.h"
# include "./parser.hpp"
#include "./request.hpp"

class CGI 
{
	private:
		std::string PATH_INFO;
		std::string REQUEST_METHOD;
		std::string SERVER_PORT;
		std::string SERVER_SOFTWARE;
		std::string GATEWAY_INTERFACE;
		std::string SCRIPT_FILENAME;
		std::string PATH_TRANSLATED;
		std::string QUERY_STRING;
		std::string REMOTE_ADDR;
		std::string REMOTE_USER;
		std::string SERVER_NAME;
		std::string REDIRECT_STATUS;
		std::string BODY;
		char		*env[14];

	public:
		CGI();
		CGI(Request request, Parser parser);

		void	set_env(char **_env);
		void	set_PATH_INFO(std::string _PATH_INFO);
		void	set_SERVER_PORT(std::string _SERVER_PORT);
		void	set_SERVER_SOFTWARE(std::string _SERVER_SOFTWARE);
		void	set_GATEWAY_INTERFACE(std::string _GATEWAY_INTERFACE);
		void	set_SCRIPT_FILENAME(std::string _SCRIPT_FILENAME);
		void	set_PATH_TRANSLATED(std::string _PATH_TRANSLATED);
		void	set_QUERY_STRING(std::string _QUERY_STRING);
		void	set_REMOTE_ADDR(std::string _REMOTE_ADDR);
		void	set_REMOTE_USER(std::string _REMOTE_USER);
		void	set_SERVER_NAME(std::string _SERVER_NAME);
		void	set_REQUEST_METHOD(std::string _REQUEST_METHOD);
		void	set_REDIRECT_STATUS(std::string _REDIRECT_STATUS);
		void	set_var(Request& request, Parser& parser);
		void	init_var();
		int	execute_cgi();	
		std::string get_body();	

		~CGI();
		CGI	&operator=(CGI& ref);
};

#endif
