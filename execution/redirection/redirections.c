/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhatahet <qhatahet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 17:25:24 by oalananz          #+#    #+#             */
/*   Updated: 2025/05/29 14:13:38 by qhatahet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_redirectout(t_shell *shell, t_token *tokens, int x)
{
	int	file_fd;

	shell->exe->count_rout++;
	file_fd = open(tokens->content[x + 1], O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (file_fd == -1)
	{
		perror("Error");
		close(file_fd);
		exit(EXIT_FAILURE);
	}
	if (shell->exe->count_rout == 1 && !is_there_redirectin(tokens)
		&& shell->exe->index != 0)
	{
		dup2(shell->exe->pipes[shell->exe->index - 1][0], STDIN_FILENO);
		close(shell->exe->pipes[shell->exe->index - 1][0]);
	}
	dup2(file_fd, STDOUT_FILENO);
	close(file_fd);
}

void	handle_append(t_shell *shell, t_token *tokens, int x)
{
	int	file_fd;

	shell->exe->count_append++;
	file_fd = open(tokens->content[x + 1], O_CREAT | O_RDWR | O_APPEND, 0644);
	if (file_fd == -1)
	{
		perror("Error");
		close(file_fd);
		exit(EXIT_FAILURE);
	}
	if (shell->exe->count_append == 1 && !is_there_redirectin(tokens)
		&& shell->exe->index != 0)
	{
		dup2(shell->exe->pipes[shell->exe->index - 1][0], STDIN_FILENO);
		close(shell->exe->pipes[shell->exe->index - 1][0]);
	}
	dup2(file_fd, STDOUT_FILENO);
	close(file_fd);
}

void	handle_redirectin(t_shell *shell, t_token *tokens, int x)
{
	int	file_fd;

	shell->exe->count_rin++;
	file_fd = open(tokens->content[x + 1], O_RDONLY);
	if (file_fd == -1)
	{
		perror("Error");
		shell->exit_status = 1;
		free_tokenizer(shell->head);
		free_shell(shell);
		free(shell);
		exit(EXIT_FAILURE);
	}
	if (shell->exe->count_rin == 1 && !is_there_redirectout(tokens)
		&& !is_there_append(tokens)
		&& shell->exe->index != shell->exe->pipes_count)
	{
		dup2(shell->exe->pipes[shell->exe->index][1], STDOUT_FILENO);
		close(shell->exe->pipes[shell->exe->index][1]);
	}
	dup2(file_fd, STDIN_FILENO);
	close(file_fd);
}
