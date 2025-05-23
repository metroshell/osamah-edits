/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   leaks_handle.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalananz <oalananz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 19:47:32 by oalananz          #+#    #+#             */
/*   Updated: 2025/05/23 19:59:17 by oalananz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_int2d(int **x)
{
	int	i;

	i = 0;
	while (x[i])
	{
		if (x[i] != NULL)
		{
			free(x[i]);
		}
		i++;
	}
	free(x);
	x = NULL;
}

void	exit_execution(t_shell *shell, t_token *tokens, t_parser *parser)
{
	if (tokens)
		free_tokenizer(tokens);
	if (shell->enviroment)
		ft_free_2d(shell->enviroment);
	if (shell->env)
		free_env(shell->env);
	if (shell->cmd_list)
		ft_free_2d(shell->cmd_list);
	if (shell->exe->cmd)
		free(shell->exe->cmd);
	if (shell->exe->pids)
		free(shell->exe->pids);
	if (shell->exe->pipes)
		ft_free_int2d(shell->exe->pipes);
	if (shell->exe)
		free(shell->exe);
	if (shell)
		free(shell);
	if (parser)
		free(parser);
}

void	exit_execute(t_shell *shell, t_token *tokens)
{
	if (tokens)
		free_tokenizer(tokens);
	if (shell->enviroment)
		ft_free_2d(shell->enviroment);
	if (shell->env)
		free_env(shell->env);
	if (shell->cmd_list)
		ft_free_2d(shell->cmd_list);
	if (shell)
		free(shell);
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
