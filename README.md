# Webserver

![datagram](https://media.geeksforgeeks.org/wp-content/uploads/Socket-Programming-in-C-C-.jpg)

## Ressources:
- [medium: server from scratch in cpp](https://medium.com/from-the-scratch/http-server-what-do-you-need-to-know-to-build-a-simple-http-server-from-scratch-d1ef8945e4fa)
- [dreamincode: server from scratch in cpp](https://www.dreamincode.net/forums/topic/222927-part-1-server-from-scratch/)
- [http Tutorial](https://www.tutorialspoint.com/http/index.htm)
- [stackoverflow sock & sockstream](https://stackoverflow.com/questions/5815675/what-is-sock-dgram-and-sock-stream)
- [use of fcntl](https://youtu.be/A5vyIcBMPKo)
- [overview](https://www.geeksforgeeks.org/socket-programming-cc/)

## Vocab:
- `OSI -> Open Systems Interconnection model`
- `4th Layer: Transport Layer:
responsible for transporting one point to another reliably and without errors.
(e.g: TCP, UDP, SPX..)`

## Variables to add in .conf:

- index
- timeout
- queue
- listen port
- host address
- access log (not mandatory)
- server name
- #comments

### Ref: Ref of a basic Webserver in CPP

the sockaddr_in structure specifies a transport addres and port for the 
[AF_INET](https://docs.microsoft.com/en-us/windows-hardware/drivers/network/af-inet)
So in this case it will be an IPv4 transport address.


`int socket(int domain, int type, int protocol);` 
the [socket](https://man7.org/linux/man-pages/man2/socket.2.html) function creates
an endpoint for communication and returns a file descriptor that refers to that endpoint.
the `domain` arg spcifies a comunicaiton domain.  (in the case of an IPV4 communication
it would be `AF_INET`) and the `type` specifies the type of socket 
(in this case it will be a SOCK_STREAM, as we want a 2-ways connection that is reliable).
Finally, the protocol will assigned to 0, so that we get the default one relatif to the address family)

```
    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt)))
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
    valread = read( new_socket , buffer, 1024);
    printf("%s\n",buffer );
    send(new_socket , hello , strlen(hello) , 0 );
    printf("Hello message sent\n");
    return 0;
}
```
