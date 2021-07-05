#include "../includes/webserver.h"

bool method_error(int *fd, Socket &sock, t_data *data)
{
	std::cout<< "HELLO IM ERROR" <<std::endl;
	send_header(sock, *fd, 0, NULL, data);
	std::cout<<"done erroring"<<std::endl;
	return (1);
}

bool method_get(int *fd, Socket &sock, t_data *data)
{
	std::cout<< "HELLO IM GET" <<std::endl;
	std::string source = sock.get_request().get_arg_method();
	clean_path(source);
	if (source.length() == 0)
		source = sock.get_parser().get_index();
	source = sock.get_parser().get_root() + source;
	send_html(*fd, &source[0], sock, data);
	std::cout<<"done getting"<<std::endl;
	return (1);
}

bool method_post(int *fd, Socket &sock, t_data *data)
{
	std::cout<< "HELLO IM POST" <<std::endl;
	std::string type = sock.get_request().get_content_type();
	std::string method = sock.get_parser().get_root();
	for(int i = 0; type[i] && type[i] != '/'; i++)
		method += type[i];
	std::cout << "file name:" << method << std::endl;

	std::fstream file;
	file.open(method, std::ios::out);
	if (!file)
		data->status = 404;
	else
		file << "POST BODY HERE";
	file.close();
	send_header(sock, *fd, 0, NULL, data);
	std::cout<<"done posting"<<std::endl;
	return (1);
}

bool method_delete(int *fd, Socket &sock, t_data *data)
{
	std::cout<< "HELLO IM DELETE" <<std::endl;
	std::string method = sock.get_request().get_arg_method();
	clean_path(method);
	std::string file = sock.get_parser().get_root() + method;

	std::cout<< "file to delete :" << file <<std::endl;
	if (std::remove(&file[0]) != 0)
		data->status = 404;
	send_header(sock, *fd, 0, NULL, data);
	std::cout<<"done deleting"<<std::endl;
	return (1);
}
