/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qais <qais@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 14:24:34 by oalananz          #+#    #+#             */
/*   Updated: 2025/05/24 12:24:15 by qais             ###   ########.fr       */
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
	int				fd_out[2];
	int				fd_heredoc;
	int				saved_out;
	int				saved_in;
	int				flag_out;
	int				flag_in;
	int				flag_append;
	int				flag_expand;
	int				flag_heredoc;
	char			*out_file;
	char			*in_file;
	char			**list;
	char			*temp;
	char			*delimiter;
	int				index_i;
	int				index_j;
	char			*text;
	pid_t			pid;
}					t_fds;

typedef struct s_execute
{
	int				index;
	int				j;
	int				pipes_count;
	char			*cmd;
	int				**pipes;
	int				*pids;
	int				count_rout;
	int				count_rin;
	int				count_append;
	int				flag;

}					t_execute;

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
	char			*cmd;
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
	t_execute		*exe;
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

typedef struct s_quote_state
{
	int				index;
	int				i;
	int				count;
	char			quote;
}					t_quote_state;

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
void				print_env(t_env *env);
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
void				execute(t_shell *shell, t_token *tokens);
void				add_backslash(t_shell *shell);
void				get_paths(t_shell *shell);
void				free_tokenizer(t_token *tokens);
void				free_env(t_env *env);
int					ft_executor(t_shell *shell, t_token *token);
void				open_heredoc(t_shell *shell, char **lst, t_fds *fds);
void				skip_spaces(t_shell *shell);
void				exit_execution(t_shell *shell, t_token *tokens);
void				execute_multiple(t_token *tokens, t_shell *shell);
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
void				execute_cmd_with_path(t_shell *shell, t_token *tokens, t_fds *fd);
void				link_cmd_with_path(t_shell *shell, t_token *tokens,
						t_fds *fd);
void				exit_cmd_not_found(t_shell *shell, t_token *tokens, t_fds *fd);
void				cleanup_execute_command(t_shell *shell, t_fds *fd);
void				get_exit_status(int id);
void				check_files_in_child(t_fds *fd);
char				**get_env(t_env *env);
int					count_content(t_token *tokens);
int					how_many_pipes(t_token *tokens);
int					redirect_first_arg(t_token *tokens);
int					is_there_redirect(t_token *tokens);
int					is_there_command(t_token *tokens);
int					is_there_heredoc(t_token *tokens);
void				path_check(t_shell *shell, t_token *tokens);
void				cmd_check(t_shell *shell);
int					counter(t_token *tokens);
void				child_process(t_shell *shell, t_token *tokens);
void				cmd_not_found(t_shell *shell);
void				exit_execute(t_shell *shell, t_token *tokens);
char				**list(t_token *tokens);
void				normal_execute(t_shell *shell, t_token *tokens);
void				ctrl_d(t_shell *shell);
void				free_loop(t_shell *shell, t_token *tokens);

// redirections
int					is_there_redirectin(t_token *tokens);
int					is_there_append(t_token *tokens);
int					is_there_redirectout(t_token *tokens);
void				handle_redirectout(t_shell *shell, t_token *tokens, int x);
void				handle_append(t_shell *shell, t_token *tokens, int x);
void				handle_redirectin(t_shell *shell, t_token *tokens, int x);
void				dup_in(t_fds *fd, int *j);
void				dup_out(t_fds *fd, int *j);
void				restore_in_out(t_fds *fd);
int					redirect(char **lst, t_shell *shell, t_fds *fd);
int					redirections(char **lst, int *i);
int					open_appendfiles(t_fds *fd, char **lst, int *i, int *j);
int					open_redirect_out(t_fds *fd, char **lst);
int					open_redirect_in(t_fds *fd, char **lst);
int					open_outfiles(t_fds *fd, char **lst, int *i, int *j);
int					open_infiles(t_fds *fd, char **lst, int *i, int *j);
char				**open_files(char **lst, t_token *tokens, t_fds *fd,
						t_shell *shell);
void				ft_free_int2d(int **x, t_shell *shell);

// heredoc
void				heredoc_handle(t_token *tokens, t_shell *shell);
int					count_heredoc(t_token *tokens);
int					open_heredocs(t_shell *shell, char *exit_heredoc,
						char *file);
int					create_heredoc_files(t_token *tokens);
char				*remove_qoutes(char *string, t_shell *shell);
char				*expand_heredoc(char *text, t_shell *shell);
void				dollar(char *text, t_shell *shell, char *temp, int *i);
int					length(char *text, t_shell *shell);
void				heredoc_ctrl_d(char *text, char *exit_heredoc);
void				handle_heredoc_child(void);
void				open_file(t_fds *fds, int j);
void				heredoc_signal_handler(int sig);

#endif
