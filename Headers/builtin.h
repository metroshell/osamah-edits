/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalananz <oalananz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 02:51:20 by oalananz          #+#    #+#             */
/*   Updated: 2025/04/01 03:18:23 by oalananz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "minishell.h"

typedef struct s_shell	t_shell;
typedef struct s_token	t_token;

typedef struct s_env
{
	char				*variable;
	char				*content;
	struct s_env		*next;
}						t_env;

// typedef struct s_export
// {
// 	int				index;
// 	char			*variable;
// 	int				find;
// }					t_export;

// // export command
// void				export_var(t_shell *shell, t_export *export);
// void				scan_env(t_shell *shell, t_export *export);
// void				add_to_env(t_shell *shell, t_export *export);
// void				replace_env(t_shell *shell, t_export *export);
// void				export_command(t_shell *shell);

// env command
void					env_copy(t_shell *shell, char **env);
void					env_command(t_shell *shell);
void					print_env(t_env *env);

// echo command
void					echo_command(t_shell *shell, t_token *token);

#endif