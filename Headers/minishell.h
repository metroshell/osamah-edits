/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qais <qais@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 14:24:34 by oalananz          #+#    #+#             */
/*   Updated: 2025/03/15 01:42:38 by qais             ###   ########.fr       */
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
	char	**commands;
	int		commands_counter;
	char	**heredocs;
	int		heredocs_counter;
	char	**redirect;
	int		redirect_counter;
	char	**filename;
	int		filename_counter;
	char	**arguments;
	int		arguments_counter;
	char	**texts;
	int		texts_counter;
	int		index;
	int		filename_flag;
	int		texts_flag;
}		t_parser;

typedef struct s_shell
{
	char	**environment;
	int		env_counter;
	char	**paths;
	char	**history;
	int		history_counter;
	char	*prompt;
	int		prompt_index;
	int		token_index;
	int		token_flag;
	int		qoute_flag;
	int		counter;
	int		temp_index;
}			t_shell;

typedef struct s_token
{
	char			**content;
	struct s_token	*next;
}		t_token;

typedef struct s_export
{
	int		index;
	char	*variable;
	int		find;
}			t_export;

typedef struct s_echo
{
	int		single_qoute;
	int		double_qoute;
	int		new_line;
	int		index;
	int		index_out;
	char	*string;
	char	*expand;
	char	*output;
}			t_echo;

// void		add_command(char *cmd, t_shell *shell);
// void		add_heredoc(char *heredoc, t_shell *shell);
// void		detect_heredocs(t_shell *shell);
// void		detect_commands(t_shell *shell);
// toknizer
t_token	*tokenizer(t_shell *shell);
t_token	*create_new_node(t_shell *shell);
void	copy(t_shell *shell, t_token *token);
void	copy_quotes(t_shell *shell, t_token *token, char detect);
void	tokenizer_size(t_shell *shell);
// parser

t_parser	*ft_parser(t_token *head, t_shell *shell);
void	add_backslash(t_shell *shell);
void	get_paths(t_shell *shell);
void	initialize_parser(t_parser *parser);
void	fill_parser(t_parser *parser);
// export command
void	export_var(t_shell *shell, t_export *export);
void	scan_env(t_shell *shell, t_export *export);
void	add_to_env(t_shell *shell, t_export *export);
void	replace_env(t_shell *shell, t_export *export);
void	export_command(t_shell *shell);
// env command
void	env_copy(t_shell *shell, char **env);
void	env_command(t_shell *shell);
// echo command
void	set_flags(t_echo *all, t_shell *shell);
void	copy_var(t_shell *shell, t_echo *all);
void	check_env(t_shell *shell, t_echo *all);
void	dollar_sign(t_shell *shell, t_echo *all);
void	echo_command(t_shell *shell);

#endif