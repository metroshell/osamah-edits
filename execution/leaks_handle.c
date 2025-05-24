/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   leaks_handle.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhatahet <qhatahet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 19:47:32 by oalananz          #+#    #+#             */
/*   Updated: 2025/05/24 15:49:23 by qhatahet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_int2d(int **x, t_shell *shell)
{
	int	i;

	i = 0;
	while (i < shell->exe->pipes_count)
	{
		if (x[i])
		{
			free(x[i]);
		}
		i++;
	}
	free(x);
	x = NULL;
}

void	exit_execution(t_shell *shell, t_token *tokens)
{
	if (tokens)
	{
		free_tokenizer(shell->head);
		tokens = NULL;
	}
	if (shell && shell->exe && shell->exe->cmd)
	{
		free(shell->exe->cmd);
		shell->exe->cmd = NULL;
	}
	if (shell && shell->enviroment)
	{
		ft_free_2d(shell->enviroment);
		shell->enviroment = NULL;
	}
	if (shell && shell->env)
	{
		free_env(shell->env);
		shell->env = NULL;
	}
	if (shell && shell->cmd_list)
	{
		ft_free_2d(shell->cmd_list);
		shell->cmd_list = NULL;
	}
	if (shell && shell->exe && shell->exe->pids)
	{
		free(shell->exe->pids);
		shell->exe->pids = NULL;
	}
	if (shell && shell->exe && shell->exe->pipes)
	{
		ft_free_int2d(shell->exe->pipes, shell);
		shell->exe->pipes = NULL;
	}
	if (shell && shell->exe)
	{
		free(shell->exe);
		shell->exe = NULL;
	}
	if (shell && shell->paths)
	{
		ft_free_2d(shell->paths);
		shell->paths = NULL;
	}
	if (shell)
	{
		free(shell);
		shell = NULL;
	}
}



void	cmd_not_found(t_shell *shell)
{
	char	*temp;
	char	*string;

	temp = ft_strjoin("command not found: ", shell->cmd_list[0]);
	string = ft_strjoin(temp, "\n");
	write(2, string, ft_strlen(string));
	free(temp);
	free(string);
}
