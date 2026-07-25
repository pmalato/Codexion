NAME = codexion

CFLAGS = -pthread -Wall -Wextra -Werror

SRC = codexion.c \
	src/parsing/parser.c \
	src/parsing/parser_utils1.c \
	src/threading/structs_config.c \
	src/thredaing/thread_routine.c
	
OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	cc $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c
	cc $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)
	clear

fclean: clean
	rm -f $(NAME)
	clear

re: fclean all

.PHONY: all clean fclean re