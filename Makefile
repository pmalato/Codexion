NAME = codexion

CFLAGS = -Wall -Wextra -Werror -c

SRC = codexion.c \
	src/parsing/parser.c \
	src/parsing/parser_utils1.c \
	src/parsing/parser_utils2.c
	
OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	ar rcs $(NAME) $(OBJ)

%.o: %.c
	cc $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re