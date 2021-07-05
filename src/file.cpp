#include "../includes/webserver.h"

int	get_file_size(const char *path)
{
	std::ifstream	file(path);
	std::string	line;
	int 		count;

	count = 0;
	std::cout<<"file size path: "<<path<<std::endl;
	if (file.is_open())
	{
		while (std::getline(file, line))
		{
			count += line.length();
		}
		file.close();
	}
	return count;
}
