NAME = codexion

CFLAGS = -pthread -Wall -Wextra -Werror

SRC = codexion.c \
	src/parsing/parser.c \
	src/parsing/parser_utils.c \
	src/threading/structs_config.c \
	src/threading/thread_config.c \
	src/threading/routine_config.c
	
OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	cc $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c
	cc $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re