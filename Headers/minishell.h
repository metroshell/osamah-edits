/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalananz <oalananz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 14:24:34 by oalananz          #+#    #+#             */
/*   Updated: 2025/03/22 23:19:34 by oalananz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>

typedef struct s_parser
{
	int				append_counter;
	int				commands_counter;
	int				heredocs_counter;
	int				redirect_counter;
	int				filename_counter;
	int				arguments_counter;
	int				texts_counter;
	int				index;
	int				filename_flag;
	int				texts_flag;
}					t_parser;

typedef struct s_shell
{
	char			**environment;
	int				env_counter;
	char			**paths;
	char			**history;
	int				history_counter;
	char			*prompt;
	int				prompt_index;
	int				token_index;
	int				token_flag;
	int				qoute_flag;
	int				counter;
	int				temp_index;
}					t_shell;

typedef enum s_type
{
	COMMAND = 0,
	ARGUMENT = 1,
	HEREDOC = 2,
	APPEND = 3,
	TEXT = 4,
	REDIRECT = 5,
	FILENAME = 6,
}					t_type;

typedef struct s_token
{
	t_type			*type;
	char			**content;
	struct s_token	*next;
}					t_token;

typedef struct s_export
{
	int				index;
	char			*variable;
	int				find;
}					t_export;

typedef struct s_echo
{
	int				single_qoute;
	int				double_qoute;
	int				new_line;
	int				index;
	int				index_out;
	char			*string;
	char			*expand;
	char			*output;
}					t_echo;

// toknizer
t_token				*tokenizer(t_shell *shell);
t_token				*create_new_node(t_shell *shell);
void				copy(t_shell *shell, t_token *token);
void				copy_quotes(t_shell *shell, t_token *token, char detect);
void				tokenizer_size(t_shell *shell);
void				check_quotes(t_shell *shell);
int					check_pipe(t_shell *shell);
int					check_quotes_norm(t_shell *shell, int *quote, int i,
						char c);
void				check_prompt_1(t_shell *shell);
void				check_prompt_2(t_shell *shell, t_token **token);
void				check_prompt_3(t_shell *shell, t_token **token);
void				check_prompt_4(t_shell *shell);
void				check_errors(t_shell *shell);

// parser
void				ft_parser(t_token *head, t_shell *shell);
void				detect_heredoc(t_parser *parser, t_token *temp);
void				detect_arguments(t_parser *parser, t_token *temp);
void				detect_command(t_parser *parser, t_token *temp,
						char **paths);
void				detect_redirect(t_parser *parser, t_token *temp);
void				detect_filename(t_parser *parser, t_token *temp);

// export command
void				export_var(t_shell *shell, t_export *export);
void				scan_env(t_shell *shell, t_export *export);
void				add_to_env(t_shell *shell, t_export *export);
void				replace_env(t_shell *shell, t_export *export);
void				export_command(t_shell *shell);

// env command
void				env_copy(t_shell *shell, char **env);
void				env_command(t_shell *shell);

// echo command
void				set_flags(t_echo *all, t_shell *shell);
void				copy_var(t_shell *shell, t_echo *all);
void				check_env(t_shell *shell, t_echo *all);
void				dollar_sign(t_shell *shell, t_echo *all);
void				echo_command(t_shell *shell);

#endif