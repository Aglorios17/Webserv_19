#include "../includes/webserver.h"


int		method_error(int *fd, Socket &sock, t_data *data)
{
	std::string s;
	int ret = 2;

	std::cout << MAGENTA << "============= ERROR =============" << RESET << std::endl;
	s = send_header(sock, *fd, 0, NULL, data);
	print_reponse(s);

	ret = send(*fd, &s[0], strlen(&s[0]), 0);
	reset_sock_request(sock);
	return (ret);
}

std::string	get_path_info(Socket &sock, int method)
{
	std::string source = sock.get_request().get_arg_method();

	clean_path(source);
	if (source.length() == 0 && method != 0)
		source = sock.get_parser().get_index();
	source = sock.get_parser().get_root() + source;//PATH_INFO

	return source;
}

int		method_get(int *fd, Socket &sock, t_data *data)
{
	std::string source = get_path_info(sock, 1);
	std::string extension;
	int ret = 2;


	if (source.find('?') != std::string::npos)
		extension= source.substr(0, source.find('?') );

	std::string cgi_extension = sock.get_parser().get_cgi_extension();
	if (cgi_extension.size() && !get_extension(strtrim(extension, '.')).compare(cgi_extension))
	{
		CGI cgi(sock.get_request(), sock.get_parser());
		ret = cgi.execute_cgi();
		std::string body = cgi.get_body();

		if (ret == 0 || ret == 1)
			return ret;
		ret = send(*fd, &body[0], strlen(&body[0]), 0);
		reset_sock_request(sock);
		return (ret);
	}		
	
	ret = file2socket(*fd, &source[0], sock, data);
	reset_sock_request(sock);
	return (ret);
}

int		method_post(int *fd, Socket &sock, t_data *data)
{

	std::string path_info = get_path_info(sock, 0);
	std::fstream file;
	file.open(path_info, std::ios::out);
	int ret = 2;
	std::string s;

	if (!file.is_open())// Check whether exist or empty (404 or 405)
	{
		data->status = 405;
		return (method_error(fd, sock, data));
	}
	else
	{
		file << sock.get_request().get_body();
		std::cout << RED << "FILE : " << path_info << " CREATED !" << RESET << std::endl;
	}
	//------------------------------------------
	file.close();
	s = send_header(sock, *fd, 0, NULL, data);
	ret = send(*fd, &s[0], strlen(&s[0]), 0);

	print_reponse(s);
	reset_sock_request(sock);
	return (ret);
}

int		method_delete(int *fd, Socket &sock, t_data *data)
{

	std::string path_info = get_path_info(sock, 0);
	std::fstream file;
	file.open(path_info);

	int ret = 2;
	std::string s;
	if (!file.good())
	{
		data->status = 404;
		return (method_error(fd, sock, data));
	}
	else
	{
		file.close();
		if (std::remove(&path_info[0]) != 0)	
		{
			data->status = 405;
			return (method_error(fd, sock, data));
		}
		else
			std::cout << RED << "FILE : " << path_info << " REMOVE !" << RESET << std::endl;
	}

	s = send_header(sock, *fd, 0, NULL, data);

	ret = send(*fd, &s[0], strlen(&s[0]), 0);
	print_reponse(s);
	reset_sock_request(sock);
	return (ret);
}
