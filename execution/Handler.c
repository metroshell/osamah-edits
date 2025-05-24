/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Handler.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qais <qais@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 14:23:53 by oalananz          #+#    #+#             */
/*   Updated: 2025/05/24 11:47:55 by qais             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_exit_status;

void	normal_execute(t_shell *shell, t_token *tokens)
{
	shell->exe->flag = 0;
	path_check(shell, tokens);
	if (shell->cmd_list[0] && !shell->exe->flag)
	{
		cmd_check(shell);
		if (!shell->exe->cmd)
		{
			cmd_not_found(shell);
			shell->exe->j = 0;
			while (shell->exe->j < shell->exe->pipes_count)
			{
				if (shell->exe->index != shell->exe->j + 1)
					close(shell->exe->pipes[shell->exe->j][0]);
				if (shell->exe->index != shell->exe->j)
					close(shell->exe->pipes[shell->exe->j][1]);
				shell->exe->j++;
			}
			exit_execution(shell, tokens);
			exit(127);
		}
	}
	signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, SIG_DFL);
	execve(shell->exe->cmd, shell->cmd_list, shell->enviroment);
}

void	exec_init(t_token *tokens, t_shell *shell)
{
	shell->exe->pipes_count = how_many_pipes(tokens);
	shell->exe->j = 0;
	shell->exe->index = 0;
	shell->exe->pipes = malloc(sizeof(int *) * shell->exe->pipes_count);
	if (shell->exe->pipes == NULL)
		return ;
	while (shell->exe->index < shell->exe->pipes_count)
	{
		shell->exe->pipes[shell->exe->index] = malloc(sizeof(int) * 2);
		if (shell->exe->pipes[shell->exe->index] == NULL)
			return ;
		if (pipe(shell->exe->pipes[shell->exe->index]) == -1)
			return ;
		shell->exe->index++;
	}
	shell->exe->index = 0;
}

void	exec_commands(t_token *tokens, t_shell *shell)
{
	t_token *temp;

	temp = tokens;
	while (temp)
	{
		child_process(shell, temp);
		signal(SIGINT, SIG_IGN);
		temp = temp->next;
		free(shell->cmd_list);
		shell->cmd_list = NULL;
		shell->exe->index++;
	}
	shell->exe->j = 0;
	while (shell->exe->j < shell->exe->pipes_count)
	{
		close(shell->exe->pipes[shell->exe->j][0]);
		close(shell->exe->pipes[shell->exe->j][1]);
		shell->exe->j++;
	}
	shell->exe->index = -1;
}

void	execute_multiple(t_token *tokens, t_shell *shell)
{
	int		status;
	pid_t	last_pid;

	shell->exe = ft_calloc(1, sizeof(t_execute));
	if (!shell->exe)
		return ;
	exec_init(tokens, shell);
	shell->exe->pids = malloc(sizeof(int) * (shell->exe->pipes_count + 2));
	if (!shell->exe->pids)
		return ;
	create_heredoc_files(tokens);
	heredoc_handle(tokens, shell);
	exec_commands(tokens, shell);
	last_pid = shell->exe->pids[shell->exe->pipes_count];
	while (++shell->exe->index < shell->exe->pipes_count + 1)
	{
		shell->exe->pids[shell->exe->index] = wait(&status);
		if (shell->exe->pids[shell->exe->index] == last_pid)
		{
			if (WIFEXITED(status))
			shell->exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
			shell->exit_status = 128 + WTERMSIG(status);
		}
	}
	if (shell && shell->exe && shell->exe->pipes)
		ft_free_int2d(shell->exe->pipes, shell);
	if (shell && shell->exe && shell->exe->pids)
		free(shell->exe->pids);
	if (shell && shell->exe)
		free (shell->exe);
}
