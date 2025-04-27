CC = cc
NAME = pipex
CFLAGS = -Wall -Wextra -Werror

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

SRC = pipex.c validation.c pipeing.c
OBJ = $(SRC:.c=.o)

all: $(LIBFT) $(NAME)

$(NAME): $(OBJ) $(LIBFT) pipex.h Makefile
	$(CC) $(OBJ) $(LIBFT) -o $(NAME) 
#-g3 -fsanitize=address

%.o: %.c
	$(CC) $(CFLAGS) -I$(LIBFT_DIR) -c $< -o $@

$(LIBFT):
	make -C $(LIBFT_DIR)

clean:
	rm -f $(OBJ)
	make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME) 
	make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re