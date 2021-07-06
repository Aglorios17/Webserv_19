#include "../includes/webserver.h"

bool method_error(int *fd, Socket &sock, t_data *data)
{
	std::cout<< "HELLO IM ERROR" <<std::endl;
	send_header(sock, *fd, 0, NULL, data);
	std::cout<<"done erroring"<<std::endl;
	return (1);
}

std::string get_path_info(Socket &sock, int method)
{
	std::string source = sock.get_request().get_arg_method();

	clean_path(source);
	if (source.length() == 0 && method != 0)
		source = sock.get_parser().get_index();
	source = sock.get_parser().get_root() + source;//PATH_INFO

	return source;
}

bool method_get(int *fd, Socket &sock, t_data *data)
{
	std::cout<< "GET METHOD: " <<std::endl;

	std::string source = get_path_info(sock, 1);
	std::string extension = source;

	std::cout << " file extension >>>>>>>>>>>>>>> " << get_extension(strtrim(extension, '.')) << std::endl;
	std::cout << "cgi extension	>>>>>>>>>>>>> " << sock.get_parser().get_cgi_extension();

	if (!get_extension(strtrim(extension, '.')).compare(sock.get_parser().get_cgi_extension()))
	{
		std::cout<<"LAUNCH CGI!!!"<<std::endl;	
		CGI cgi(sock.get_request(), sock.get_parser());
	}		

	send_html(*fd, &source[0], sock, data);//should send_file
	std::cout<<"done getting"<<std::endl;
	return (1);
}

bool method_post(int *fd, Socket &sock, t_data *data)
{
	std::cout<< "POST METHOD: " <<std::endl;

	std::string path_info = get_path_info(sock, 0);
	std::cout << ">>>>>>>>>>>>>>>file name:" << data << std::endl;
	
	std::fstream file;
	file.open(path_info, std::ios::out);
	/*
	 * check if:
	 * file string empty -> 405
	 * path/to/file is forbidden -> 405
	 */

	if (path_info.empty())// Check whether exist or empty (404 or 405)
		data->status = 405;
	else
	{
		std::cout << "BODY : ||" << sock.get_request().get_body() << "||\n";
		file << sock.get_request().get_body();
	}
	//------------------------------------------
	file.close();
	send_header(sock, *fd, 0, NULL, data);
	std::cout<<"done posting"<<std::endl;
	return (1);
}

bool method_delete(int *fd, Socket &sock, t_data *data)
{
	std::cout<< "HELLO IM DELETE" <<std::endl;

	std::string path_info = get_path_info(sock, 0);
	std::cout << ">>>>>>>>>>>>>>>file name:" << path_info << std::endl;

// Check whether exist or empty (404 or 405) (cant delete whole dir or sensitive files
	if (std::remove(&path_info[0]) != 0)	
		data->status = 405;
	send_header(sock, *fd, 0, NULL, data);
	std::cout<<"done deleting"<<std::endl;
	return (1);
}
