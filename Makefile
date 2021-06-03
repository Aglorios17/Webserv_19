TARGET = webserver

DIR = src/

CC = clang++

SRCS  = main.cpp\

OBJS  =  $(DIR)$(SRCS:%.cpp=%.o)

CFLAG =  -Wall -Wextra -Werror -std=c++98

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $<

%.o: %.cpp
	$(CC) $(CCFLAGS) -c -o $@ $^

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(TARGET)
re:
	fclean all
