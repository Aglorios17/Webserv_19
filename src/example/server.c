#include <stdio.h>
#define BUF 10000
#include "../header/webserv.h"

int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUF] = {0};
    char *hello = "Hello from server";



		
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
  //  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
     //                                             &opt, sizeof(opt)))
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR , &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address,
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
   	while (1)
	{
    	if (listen(server_fd, 3) < 0)
    	{
    	    perror("listen");
    	    exit(EXIT_FAILURE);
    	}
    	if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
    	                   (socklen_t*)&addrlen))<0)
    	{
    	    perror("accept");
    	    exit(EXIT_FAILURE);
    	}
		printf("==============\n");
		valread = read( new_socket , buffer, BUF);
    	printf("%s\n",buffer );
		printf("============\n");
//=============================
		const char *s1 = "HTTP/1.1 200 OK\nContent-length: 0\n";
		const char *s2 = "Content-Type: text/html\n\n";

    	send(new_socket , s1, strlen(s1) , 0 );
    	send(new_socket , s2, strlen(s2) , 0 );

    	printf("Hello message sent\n");
	}
    return 0;
}
