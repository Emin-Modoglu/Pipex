CC := cc
CFLAGS := -Wall -Wextra -Werror
SRCS :=	pipex.c
NAME := pipex
OBJS := ${SRCS:.c=.o}

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(SRCS) -o $(NAME)
clean :
	rm -f ${OBJS}
fclean : clean
	rm -f $(NAME)
re : fclean all

.PHONY : all clean fclean re
