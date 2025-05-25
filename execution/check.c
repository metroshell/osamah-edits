/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhatahet <qhatahet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 19:49:57 by oalananz          #+#    #+#             */
/*   Updated: 2025/05/25 14:42:55 by qhatahet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	path_check(t_shell *shell, t_token *tokens)
{
	get_paths(shell);
	if (!shell->paths)
	{
		cmd_not_found(shell);
		exit_execution(shell, tokens);
		exit(127);
	}
	if (!shell->enviroment)
		shell->enviroment = get_env(shell->env);
	if (shell->cmd_list[0] && (shell->cmd_list[0][0] == '.'
		|| shell->cmd_list[0][0] == '/'))
	{
		if (!access(shell->cmd_list[0], X_OK))
			shell->exe->cmd = ft_strdup(shell->cmd_list[0]);
		else if (shell->cmd_list[0][0] == '/')
		{
			exit_cmd_not_found(shell, shell->head, NULL);
		}
		else if (shell->cmd_list[0][0] == '.')
		{
			exit_cmd_not_found(shell, shell->head, NULL);
		}
		shell->exe->flag = 1;
	}
}

void	cmd_check(t_shell *shell)
{
	shell->exe->j = 0;
	while (shell->paths && shell->paths[shell->exe->j])
	{
		shell->exe->cmd = ft_strjoin(shell->paths[shell->exe->j],
				shell->cmd_list[0]);
		if (!shell->exe->cmd)
			exit(EXIT_FAILURE);
		if (!access(shell->exe->cmd, X_OK))
			break ;
		free(shell->exe->cmd);
		shell->exe->cmd = NULL;
		shell->exe->j++;
	}
	if (shell->paths)
	{
		ft_free_2d(shell->paths);
		shell->paths = NULL;
	}
}

static int	count_norm(t_token *tmp, int *i, int count)
{
	if (tmp->type[*i] == HEREDOC)
	{
		(*i)++;
		count++;
	}
	else if (tmp->type[*i] == REDIRECTIN || tmp->type[*i] == REDIRECTOUT
		|| tmp->type[*i] == APPEND || tmp->type[*i] == ENDOFFILE
		|| tmp->type[*i] == FILENAME)
		(*i)++;
	else if (tmp->type[*i] == COMMAND || tmp->type[*i] == ARGUMENT
		|| tmp->type[*i] == TEXT)
	{
		count++;
		(*i)++;
	}
	return (count);
}

int	counter(t_token *tokens)
{
	t_token	*tmp;
	int		i;
	int		count;

	tmp = tokens;
	i = 0;
	count = 0;
	while (tmp->content[i])
		count += count_norm(tmp, &i, count);
	if (is_there_command(tmp) && is_there_heredoc(tmp))
		count++;
	return (count);
}
