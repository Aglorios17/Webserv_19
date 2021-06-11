TARGET = webserver

SRC_DIR = src

CC = clang++

SRCS  = $(SRC_DIR)/parser.cpp\
	$(SRC_DIR)/socket.cpp\
	$(SRC_DIR)/request.cpp\
	$(SRC_DIR)/webserver.cpp\
	$(SRC_DIR)/configure.cpp\
	$(SRC_DIR)/connect.cpp\
	$(SRC_DIR)/file.cpp


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

run: all
	./webserver src/conf/server1.conf
re:	fclean all
