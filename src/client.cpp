#include "../includes/webserver.h"

#define PORT 8080

int main()
{
    int sock = 0;
    struct sockaddr_in serv_addr;
    std::string hello = "Hello from client";
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
    printf("Hello message sent\n");
    read( sock , buffer, 1024);
    printf("%s\n",buffer );
    return 0;
}
