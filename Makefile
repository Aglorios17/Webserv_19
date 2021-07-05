# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: elajimi <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/06/07 16:08:04 by elajimi           #+#    #+#              #
#    Updated: 2021/06/15 15:03:07 by elajimi          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

TARGET = webserver

SRC_DIR = src

CC = clang++

SRCS  = $(SRC_DIR)/parser.cpp\
		$(SRC_DIR)/request.cpp\
		$(SRC_DIR)/utils.cpp\
		$(SRC_DIR)/socket.cpp\
		$(SRC_DIR)/webserver.cpp\
		$(SRC_DIR)/configure.cpp\
		$(SRC_DIR)/connect.cpp\
		$(SRC_DIR)/file.cpp\
		$(SRC_DIR)/poll.cpp\
		$(SRC_DIR)/method.cpp\
		$(SRC_DIR)/requesthandler.cpp


OBJS  =  $(SRCS:%.cpp=%.o)

CFLAG =  -g -Wall -Wextra -Werror -std=c++98

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^

%.o: %.cpp
	$(CC) $(CFLAG) -c -o $@ $^

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(TARGET)
	rm ./client | true

run: all
	./webserver includes/conf/tester.conf
client: all
	clang++ -Wall -Wextra -Werror -std=c++98 src/client.cpp -o client
	echo "./webserver includes/conf/tester.conf"
valgrind: all
	valgrind ./webserver includes/conf/tester.conf
tester: all
	chmod +x ./tester/tester
	./tester/tester http://localhost:8080

re:	fclean all
