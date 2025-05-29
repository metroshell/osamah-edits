/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qais <qais@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 02:10:24 by oalananz          #+#    #+#             */
/*   Updated: 2025/05/29 10:20:11 by qais             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		if (env->variable)
			free(env->variable);
		if (env->content)
			free(env->content);
		free(env);
		env = tmp;
	}
	free(env);
}

void	free_tokenizer(t_token *tokens)
{
	t_token	*temp;

	while (tokens)
	{
		temp = tokens->next;
		if (tokens->content)
			ft_free_2d(tokens->content);
		if (tokens->type)
			free(tokens->type);
		if (tokens->heredoc_file)
		{
			// if (!access(tokens->heredoc_file, F_OK))
			// 	unlink(tokens->heredoc_file);
			free(tokens->heredoc_file);
		}
		free(tokens);
		tokens = temp;
	}
	free(tokens);
	tokens = NULL;
}

void	close_pipes(int **pipe, t_shell *shell)
{
	int	i;

	i = 0;
	if (!pipe)
		return ;
	while (i < shell->exe->pipes_count)
	{
		if (pipe[i][0] && pipe[i][0] > 0)
			close(pipe[i][0]);
		if (pipe[i][1] && pipe[i][1] > 0)
			close(pipe[i][1]);
		i++;
	}
}

void	free_shell(t_shell *shell)
{
	if (!shell)
		return ;
	if (shell->head)
		free_tokenizer(shell->head);
	if (shell->cmd_list)
		ft_free_2d(shell->cmd_list);
	if (shell->paths)
		ft_free_2d(shell->paths);
	if (shell->prompt)
		free(shell->prompt);
	if (shell->enviroment)
		ft_free_2d(shell->enviroment);
	if (shell->variable)
		free(shell->variable);
	if (shell->env)
		free_env(shell->env);
	if (shell->exe && shell->exe->pids)
		free(shell->exe->pids);
	if (shell->exe && shell->exe->pipes)
	{
		close_pipes(shell->exe->pipes, shell);
		ft_free_int2d(shell->exe->pipes, shell);
	}
	if (shell->exe)
		free(shell->exe);
	if (shell)
		free(shell);
}

int	valid_arg(char *status)
{
	int	i;

	i = 0;
	while (status[i])
	{
		if (ft_isdigit(status[i]))
			i++;
		else
			return (-1);
	}
	return (1);
}
