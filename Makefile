NAME = philo

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread -g
INCLUDES = -Iinclude

SRC =	src/main.c\
		src/utils.c\
		src/utilsII.c\
		src/ft_init.c\
		src/ft_validate.c\
		src/ft_monitor.c\
		src/ft_routine.c\
		

OBJ = $(SRC:.c=.o)

RM = rm -f

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
