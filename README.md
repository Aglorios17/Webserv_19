# Webserver
> Contributors: Aglorios, Elajimi

Welcome to **webserver**!
![datagram](https://media.geeksforgeeks.org/wp-content/uploads/Socket-Programming-in-C-C-.jpg)


__Here is a (somewhat) curated list of all the thing to do__:

- Organize fd multiplexer for queueing requests (note: epoll is not macos compatible).
- Make a parser and refer to the [conf list](#conf) for the variables to parse. (note: parsing can be done using no mutation by 
retrieving variables and sending it to try/catch, no need to store them temporarilly.)
- implement POST request using *recv*.
...




## Ressources:
- [HTTP parsing](https://www.cs.cmu.edu/~srini/15-441/F11/lectures/r05-http.pdf)
- [HTTP return code](https://developer.mozilla.org/en-US/docs/Web/HTTP/Status)
- [HTTP client/server/CGI principles](http://evc-cit.info/cit042/how_cgi_works.html)
- [medium: server from scratch in cpp](https://medium.com/from-the-scratch/http-server-what-do-you-need-to-know-to-build-a-simple-http-server-from-scratch-d1ef8945e4fa)
- [dreamincode: server from scratch in cpp](https://www.dreamincode.net/forums/topic/222927-part-1-server-from-scratch/)
- [http Tutorial](https://www.tutorialspoint.com/http/index.htm)
- [stackoverflow sock & sockstream](https://stackoverflow.com/questions/5815675/what-is-sock-dgram-and-sock-stream)
- [use of fcntl](https://youtu.be/A5vyIcBMPKo)
- [overview](https://www.geeksforgeeks.org/socket-programming-cc/)
- [poll/epoll/kqeue](https://copyconstruct.medium.com/the-method-to-epolls-madness-d9d2d6378642)
- [I/O multiplexing poll vs select](http://byteliu.com/2019/05/08/LINUX-%E2%80%93-IO-MULTIPLEXING-%E2%80%93-SELECT-VS-POLL-VS-EPOLL/)
- [Overview sockets http](http://www.cs.columbia.edu/~jae/3157/files/overview-sockets-http.pdf)
- [poll/epoll/select](https://www.softprayog.in/programming/io-multiplexing-select-poll-epoll-in-linux)
- [more onto poll](https://www.reddit.com/r/C_Programming/comments/eeqi8r/understanding_how_to_use_poll_in_socket/)

## Variables to add in .conf:

- index
- timeout
- queue
- listen port
- host address
- access log (not mandatory)
- server name
- #comments

