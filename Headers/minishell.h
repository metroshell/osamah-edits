/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalananz <oalananz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 14:24:34 by oalananz          #+#    #+#             */
/*   Updated: 2025/05/20 20:15:56 by oalananz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <dirent.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>

extern int			g_exit_status;

typedef struct s_env
{
	char			*variable;
	char			*content;
	struct s_env	*next;
}					t_env;

typedef struct s_heredoc
{
	char			**heredocs;
	char			**exit;
}					t_heredoc;

typedef struct s_fds
{
	int				fd_in[2];
	int				fd_out;
	int				fd_heredoc;
	int				saved_out;
	int				saved_in;
	int				flag_out;
	int				flag_in;
	int				flag_expand;
	int				flag_heredoc;
	char			**list;
	char			*temp;
}					t_fds;

typedef struct s_export
{
	int				index;
	char			*variable;
	char			*content;
	int				flag;
}					t_export;

typedef struct s_parser
{
	int				index;
	int				filename_flag;
	int				command_flag;
	int				eof_flag;
}					t_parser;

typedef struct s_shell
{
	char			**cmd_list;
	char			**paths;
	char			*prompt;
	char			**enviroment;
	int				prompt_index;
	int				token_index;
	int				token_flag;
	int				qoute_flag;
	int				counter;
	int				temp_index;
	char			*variable;
	int				echo_flag;
	int				quote_counter;
	int				expand_flag;
	int				fd_out;
	int				exit_status;
	t_env			*env;
}					t_shell;

typedef enum s_type
{
	TEXT,
	FILENAME,
	COMMAND,
	ARGUMENT,
	HEREDOC,
	APPEND,
	REDIRECTOUT,
	REDIRECTIN,
	ENDOFFILE
}					t_type;

typedef struct s_token
{
	char			*heredoc_file;
	t_type			*type;
	char			**content;
	struct s_token	*next;
}					t_token;

typedef struct s_expand
{
	int				out;
	int				in;
	int				flag;
	char			quote;
	char			*variable;
	int				single_qoute;
	int				dex;
}					t_expand;

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
void				check_filename(t_shell *shell);

// parser
void				ft_parser(t_token *head, t_parser *parser, t_shell *shell);
void				detect_heredoc(t_parser *parser, t_token *temp);
void				detect_arguments(t_parser *parser, t_token *temp);
void				detect_command(t_parser *parser, t_token *temp,
						char **paths);
void				detect_redirect(t_parser *parser, t_token *temp);
int					detect_filename(t_parser *parser, t_token *temp);

// expander
void				ft_expander(t_shell *shell, t_token *token);
int					count_quotes(t_token *token, t_expand *expand);
void				expand_dollar(t_shell *shell, t_token *token,
						t_expand *expand);
void				expand_dollar(t_shell *shell, t_token *token,
						t_expand *expand);
void				check_cmd(t_token *token, t_expand *expand, char **paths);
void				quote_remover(t_token *token, t_expand *expand);
int					numlen(int n);
int					count_length(t_shell *shell, t_token *token,
						t_expand *expand);
void				handle_dollar(t_shell *shell, t_token *token,
						t_expand *expand, char *temp);

// env command
void				env_copy(t_shell *shell, char **env);
void				print_env(t_shell *shell, t_env *env);
void				env_edit(t_shell *shell);
t_env				*create_env_node(void);

// echo command
void				echo_command(t_shell *shell, t_token *token);

// export command
void				ft_free_2d(char **a);
void				sorted_print(t_env *env);
void				export_command(t_shell *shell, t_token *token);
void				scan_env(t_shell *shell, t_export *export);
int					env_count(t_env *env);
void				concat_content(t_shell *shell, t_export *export);
void				edit_content(t_shell *shell, t_export *export);
void				env_shlvl(t_shell *shell);
void				add_variable(t_shell *shell, t_export *export);

// unset command
void				unset_command(t_shell *shell, t_token *token);

// pwd and cd
void				ft_pwd(void);
void				ft_cd(t_shell *shell, t_token *token);

// signal and exit status
void				signal_handler(void);
void				ft_exit(t_token *token, t_shell *shell);

// exit
void				free_env(t_env *env);
void				free_tokenizer(t_token *tokens);
void				free_shell(t_shell *shell);
int					valid_arg(char *status);

// execute
void				execute(t_shell *shell, t_token *tokens, t_parser *parser);
void				add_backslash(t_shell *shell);
void				get_paths(t_shell *shell);
void				free_tokenizer(t_token *tokens);
void				free_env(t_env *env);
int					ft_executor(t_shell *shell, t_token *token);
void				open_heredoc(t_shell *shell, char **lst, t_fds *fds);
void				skip_spaces(t_shell *shell);
void				exit_execution(t_shell *shell, t_token *tokens,
						t_parser *parser);
void				execute_multiple(t_token *tokens, t_shell *shell,
						t_parser *parser);
char				**get_env(t_env *env);
char				**create_list(t_token *tokens, t_fds *fd, t_shell *shell);
int					how_many_pipes(t_token *tokens);
char				**rearrange_list(t_token *tokens);
int					is_there_redirect(t_token *tokens);
char				*expand_heredoc(char *text, t_shell *shell);
char				*remove_qoutes(char *string, t_shell *shell);
int					is_there_command(t_token *tokens);
char				**rearrange_list(t_token *tokens);
int					redirect_first_arg(t_token *tokens);
char				**rearrange_list_redirect(t_token *tokens);
int					is_there_heredoc(t_token *tokens);
int					create_heredoc_files(t_token *tokens);

#endif
