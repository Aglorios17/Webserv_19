TARGET = webserver

DIR = src/

CC = clang++

SRCS  = $(DIR)Parser.cpp\
	$(DIR)Socket.cpp\
	$(DIR)webServer.cpp


OBJS  =  $(SRCS:%.cpp=%.o)

CFLAG =  -Wall -Wextra -Werror -std=c++98

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^

%.o: %.cpp
	$(CC) $(CFLAG) -c -o $@ $^

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(TARGET)
re:
	fclean all
