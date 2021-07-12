#include "../includes/webserver.h"

int	get_file_size(const char *path)
{
	std::ifstream	file(path);
	std::string	line;
	struct stat result;
	int 		count;

	count = 0;
	if (stat(path, &result) == 0)
		return result.st_size;
	std::cout<<"AN ERROR OCCURED READING FILE SIZE of "<<path<<std::endl;
	return count;
}
