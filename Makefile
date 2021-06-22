EXEC = pipex

SRCS = main.c utils.c ft_split.c

OBJS = $(SRCS:.c=.o)

CC = gcc
CFLAGS = -Wall -Wextra -Werror

$(EXEC) : $(OBJS)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJS)

all : $(EXEC)

clean :
	rm -f $(OBJS)

fclean : clean
	rm -f $(EXEC)

re : fclean all