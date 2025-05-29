/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_handle.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhatahet <qhatahet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 19:43:32 by oalananz          #+#    #+#             */
/*   Updated: 2025/05/29 18:59:05 by qhatahet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	link_cmd_with_path(t_shell *shell, t_token *tokens, t_fds *fd)
{
	int	j;

	j = 0;
	while (shell->paths && shell->paths[j])
	{
		shell->cmd = ft_strjoin(shell->paths[j], shell->cmd_list[0]);
		if (!shell->cmd)
			exit(EXIT_FAILURE);
		if (!access(shell->cmd, X_OK))
			break ;
		free(shell->cmd);
		shell->cmd = NULL;
		j++;
	}
	if (!shell->cmd)
		exit_cmd_not_found(shell, tokens, fd);
	if (!ft_strcmp(shell->cmd_list[0], ""))
	{
		if (shell->cmd)
			free (shell->cmd);
		exit_cmd_not_found(shell, tokens, fd);
	}
}

void	exit_cmd_not_found(t_shell *shell, t_token *tokens, t_fds *fd)
{
	char	*temp;
	char	*string;

	if (shell->cmd_list[0][0] == '.')
	{
		temp = ft_strjoin("ARSSH: ", shell->cmd_list[0]);
		string = ft_strjoin(temp, ": permission denied\n");
		free(temp);
		write(2, string, ft_strlen(string));
		free(string);
		if (fd)
			free(fd);
		exit_execution(shell, tokens);
		exit(126);
	}
	temp = ft_strjoin("command not found: ", shell->cmd_list[0]);
	string = ft_strjoin(temp, "\n");
	free(temp);
	write(2, string, ft_strlen(string));
	free(string);
	if (fd)
		free(fd);
	exit_execution(shell, tokens);
	exit(127);
}

void	cleanup_execute_command(t_shell *shell, t_fds *fd)
{
	if (fd)
		restore_in_out(fd);
	if (shell->cmd_list)
	{
		ft_free_2d(shell->cmd_list);
		shell->cmd_list = NULL;
	}
	if (shell->enviroment)
	{
		ft_free_2d(shell->enviroment);
		shell->enviroment = NULL;
	}
}

void	get_exit_status(int id, t_shell *shell)
{
	int	status;

	status = 0;
	waitpid(id, &status, 0);
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		shell->exit_status = 128 + WTERMSIG(status);
}

void	check_heredoc_interrupted(t_fds *fd, t_shell *shell)
{
	if (shell && shell->heredoc_interrupted)
	{
		if (fd && fd->delimiter)
			free(fd->delimiter);
		if (fd->temp)
			free(fd->temp);
		free (fd);
		exit_execution(shell, shell->head);
		exit(130);
	}
}

void	check_files_in_child(t_fds *fd, t_shell *shell)
{
	check_heredoc_interrupted(fd, shell);
	(void)shell;
	if (fd->flag_heredoc)
	{
		fd->fd_in[0] = open(fd->temp, O_RDONLY);
		fd->flag_in = 1;
		fd->saved_in = dup(STDIN_FILENO);
		unlink(fd->temp);
		dup2(fd->fd_in[0], STDIN_FILENO);
		close(fd->fd_in[0]);
	}
	else if (fd->temp)
	{
		unlink(fd->temp);
		free (fd->temp);
	}
	if (fd->flag_out || fd->flag_append)
	{
		close(fd->fd_out[0]);
		close(fd->saved_out);
	}
	if (fd->flag_in)
	{
		close(fd->fd_in[0]);
		close(fd->saved_in);
	}
}
