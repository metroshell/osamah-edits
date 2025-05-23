/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_handle.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalananz <oalananz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 19:43:32 by oalananz          #+#    #+#             */
/*   Updated: 2025/05/23 19:44:39 by oalananz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	link_cmd_with_path(t_shell *shell, t_token *tokens, t_fds *fd,
		t_parser *parser)
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
		exit_cmd_not_found(shell, tokens, parser, fd);
	free(fd);
}

void	exit_cmd_not_found(t_shell *shell, t_token *tokens, t_parser *parser,
		t_fds *fd)
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
		free(fd);
		exit_execution(shell, tokens, parser);
		exit(126);
	}
	temp = ft_strjoin("command not found: ", shell->cmd_list[0]);
	string = ft_strjoin(temp, "\n");
	free(temp);
	write(2, string, ft_strlen(string));
	free(string);
	free(fd);
	exit_execution(shell, tokens, parser);
	exit(127);
}

void	cleanup_execute_command(t_shell *shell, t_fds *fd)
{
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

void	get_exit_status(int id)
{
	int	status;

	waitpid(id, &status, 0);
	if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		g_exit_status = 128 + WTERMSIG(status);
}

void	check_files_in_child(t_fds *fd)
{
	int	fds;

	fds = 0;
	if (fd->flag_heredoc)
	{
		fds = open(fd->temp, O_RDONLY);
		fd->flag_in = 1;
		fd->saved_in = dup(STDIN_FILENO);
		unlink(fd->temp);
		dup2(fds, STDIN_FILENO);
		close(fds);
		free(fd->temp);
	}
	if (fd->flag_out && fd->flag_append)
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
