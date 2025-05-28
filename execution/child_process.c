/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qais <qais@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 19:49:07 by oalananz          #+#    #+#             */
/*   Updated: 2025/05/28 05:41:03 by qais             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	child_process_2(t_token *tokens, t_shell *shell)
{
	shell->cmd_list = list(tokens);
	shell->exe->j = 0;
	while (shell->exe->j < shell->exe->pipes_count)
	{
		if (shell->exe->index != shell->exe->j + 1)
			close(shell->exe->pipes[shell->exe->j][0]);
		if (shell->exe->index != shell->exe->j)
			close(shell->exe->pipes[shell->exe->j][1]);
		shell->exe->j++;
	}
}

void	child_process_3(t_token *tokens, t_shell *shell, int flag)
{
	int	x;

	x = 0;
	if (flag == 1)
	{
		dup2(shell->exe->pipes[shell->exe->index][1], STDOUT_FILENO);
		close(shell->exe->pipes[shell->exe->index][1]);
	}
	shell->exe->count_rout = 0;
	shell->exe->count_rin = 0;
	shell->exe->count_append = 0;
	while (tokens->content[x])
	{
		if (tokens->type[x] == REDIRECTOUT)
			handle_redirectout(shell, tokens, x);
		else if (tokens->type[x] == APPEND)
			handle_append(shell, tokens, x);
		else if (tokens->type[x] == REDIRECTIN)
			handle_redirectin(shell, tokens, x);
		// else if (tokens->type[x] == HEREDOC)
		// {
		// 	if (tokens->heredoc_file)
		// 		unlink(tokens->heredoc_file);
		// }
		x++;
	}
}

void	dup_and_close(t_shell *shell)
{
	dup2(shell->exe->pipes[shell->exe->index - 1][0], STDIN_FILENO);
	close(shell->exe->pipes[shell->exe->index - 1][0]);
	if (shell->exe->index != shell->exe->pipes_count)
	{
		dup2(shell->exe->pipes[shell->exe->index][1], STDOUT_FILENO);
		close(shell->exe->pipes[shell->exe->index][1]);
	}
}

void	child_process_4(t_token *tokens, t_shell *shell)
{
	if (shell->exe->index == 0)
	{
		if (is_there_redirect(tokens))
			child_process_3(tokens, shell, 1);
		else
		{
			dup2(shell->exe->pipes[shell->exe->index][1], STDOUT_FILENO);
			close(shell->exe->pipes[shell->exe->index][1]);
		}
	}
	else if (shell->exe->index == shell->exe->pipes_count)
	{
		if (is_there_redirect(tokens))
			child_process_3(tokens, shell, 0);
		else
			dup_and_close(shell);
	}
	else
	{
		if (is_there_redirect(tokens))
			child_process_3(tokens, shell, 1);
		else
			dup_and_close(shell);
	}
}

void	child_process(t_shell *shell, t_token *tokens)
{
	shell->exe->pids[shell->exe->index] = fork();
	if (shell->exe->pids[shell->exe->index] == -1)
	{
		write(2, "Error with creating process\n", 29);
		return ;
	}
	if (shell->exe->pids[shell->exe->index] == 0)
	{
		child_process_2(tokens, shell);
		child_process_4(tokens, shell);
		if (ft_executor(shell, tokens, NULL))
			exit(0);
		else
			normal_execute(shell, tokens);
		exit(1);
	}
}
