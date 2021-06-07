# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: elajimi <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/06/07 16:08:04 by elajimi           #+#    #+#              #
#    Updated: 2021/06/07 16:08:09 by elajimi          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

TARGET = webserver

SRC_DIR = src

CC = clang++

SRCS  = $(SRC_DIR)/parser.cpp\
		$(SRC_DIR)/socket.cpp\
		$(SRC_DIR)/webserver.cpp\
		$(SRC_DIR)/configure.cpp\
		$(SRC_DIR)/connect.cpp\
		$(SRC_DIR)/file.cpp\
		$(SRC_DIR)/poll.cpp


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
re:	fclean all
