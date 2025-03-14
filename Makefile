CC = cc

CFLAGS = -Werror -Wextra -Wall -g -IHeaders

LIBFT_DIR = libft

LIBFT = $(LIBFT_DIR)/libft.a

SRCS = main.c tokenizer.c token_utils.c token_size.c

OBJS = $(SRCS:.c=.o)

NAME = minishell

all: $(NAME) 

$(NAME) : $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS)  $(OBJS) -lreadline -L$(LIBFT_DIR) -lft -o $(NAME)

$(LIBFT):
	$(MAKE) bonus -C $(LIBFT_DIR)

clean:
	rm -f $(OBJS)
	$(MAKE) clean -C $(LIBFT_DIR)

fclean: clean
	rm -f $(NAME)
	$(MAKE) fclean -C $(LIBFT_DIR)
re: fclean all

.PHONY:fclean all clean re
