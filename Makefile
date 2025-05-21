CC = cc

CFLAGS = -Werror -Wextra -Wall -g -IHeaders #-fsanitize=address

LIBFT_DIR = libft

LIBFT = $(LIBFT_DIR)/libft.a

SRCS = main.c signals.c\
    ./built-in/environment/env.c ./built-in/environment/env_utils.c\
	./tokenizer/tokenizer.c ./tokenizer/token_utils.c ./tokenizer/token_size.c ./tokenizer/prompt_check.c \
	./parser/detect_type.c ./parser/parser.c \
	./expander/expander.c  ./expander/expander_quotes.c ./expander/count_dollar.c ./expander/expand_dollar.c ./expander/handle_dollar.c \
	./built-in/echo/echo_command.c \
	./built-in/export/export.c ./built-in/export/export_utils.c ./built-in/export/export_norm.c \
	./built-in/unset.c ./built-in/cd.c ./built-in/pwd.c \
	./built-in/exit/exit.c ./built-in/exit/exit_utils.c \
	./execution/execution.c ./execution/HereDoc/heredoc.c ./execution/Handler.c \
	./execution/redirection/redirections.c ./execution/redirection/redirect_utils.c

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
	rm -rf $(OBJS_DIR)
	$(MAKE) fclean -C $(LIBFT_DIR)
re: fclean all

.PHONY:fclean all clean re
