#include "../includes/webserver.h"

#define PORT 8080

int main(int argc, char **argv)
{
    int sock = 0;
    struct sockaddr_in serv_addr;
	if (argc != 2)
		return (-1);
	std::string hello;
    std::ifstream copy(argv[1]);
 
    std::cout << "FILE : " << argv[1] << "\n";
    if(!copy.is_open())
	{
        std::cout << "CANT OPEN \n";
		return (-1);
	}
	std::string line;
    while(std::getline(copy,line))
		 hello += line + "\n";
    std::cout << "SEND BY CLIENT \n";
    std::cout << "============================== \n";
	std::cout << hello << "\n";
	copy.close();
 
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
    
    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
	int optval = 1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int));
	bind(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    send(sock , &hello[0] , strlen(&hello[0]) , 0 );
    std::cout << "============================== \n";
    printf("MESSAGE SEND\n");
    std::cout << "------------------------------ \n";
    printf("RESPOND SERVER\n");
    read( sock , buffer, 1024);
    std::cout << "============================== \n";
    printf("%s\n",buffer );
    std::cout << "============================== \n";
    return 0;
}
