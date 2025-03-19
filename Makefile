CC = cc

CFLAGS = -Werror -Wextra -Wall -g -IHeaders

LIBFT_DIR = libft

LIBFT = $(LIBFT_DIR)/libft.a

SRCS = main.c \
	./tokenizer/tokenizer.c ./tokenizer/token_utils.c ./tokenizer/token_size.c \
	./parser/parser_utils.c ./parser/parser.c 

OBJS_DIR := objects
OBJS = $(SRCS:%.c=$(OBJS_DIR)/%.o)

NAME = minishell

all: $(NAME) 

$(NAME) : $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS)  $(OBJS) -lreadline -L$(LIBFT_DIR) -lft -o $(NAME)

$(LIBFT):
	$(MAKE) bonus -C $(LIBFT_DIR)

$(OBJS_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)
	$(MAKE) clean -C $(LIBFT_DIR)

fclean: clean
	rm -f $(NAME)
	$(MAKE) fclean -C $(LIBFT_DIR)
re: fclean all

.PHONY:fclean all clean re
