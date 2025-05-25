/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhatahet <qhatahet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 14:22:32 by qhatahet          #+#    #+#             */
/*   Updated: 2025/05/25 14:30:05 by qhatahet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Headers/minishell.h"

extern int	g_exit_status;

void	execute_cmd_with_path(t_shell *shell, t_token *tokens, t_fds *fd)
{
	if (!access(shell->cmd_list[0], X_OK))
		shell->cmd = ft_strdup(shell->cmd_list[0]);
	else
		exit_cmd_not_found(shell, tokens, fd);
}

int	execute_built_in(t_shell *shell, t_token *tokens, t_fds *fd)
{
	if (ft_executor(shell, tokens))
	{
		if (shell->fd_out)
			close(shell->fd_out);
		if (fd->flag_out || fd->flag_append)
		{
			dup2(fd->saved_out, STDOUT_FILENO);
			close(fd->saved_out);
		}
		ft_free_2d(shell->cmd_list);
		if (fd)
			free(fd);
		return (1);
	}
	return (0);
}

void	execute_child(t_shell *shell, t_token *tokens, t_fds *fd)
{
	shell->exit_status = 0;
	// g_exit_status = 0;
	check_files_in_child(fd);
	get_paths(shell);
	if (!shell->paths)
	{
		shell->paths = NULL;
		exit_cmd_not_found(shell, tokens, fd);
	}
	if (!shell->enviroment)
		shell->enviroment = get_env(shell->env);
	if (shell->cmd_list[0] && (shell->cmd_list[0][0] == '.'
		|| shell->cmd_list[0][0] == '/'))
		execute_cmd_with_path(shell, tokens, fd);
	else if (shell->cmd_list[0])
		link_cmd_with_path(shell, tokens, fd);
	else
	{
		exit_execution(shell, tokens);
		free(fd);
		exit(0);
	}
	signal(SIGQUIT, SIG_DFL);
	execve(shell->cmd, shell->cmd_list, shell->enviroment);
}

void	execute_command(t_token *tokens, t_shell *shell)
{
	t_fds	*fd;
	int		id;

	fd = ft_calloc(1, sizeof(t_fds));
	fd->temp = NULL;
	if (!fd)
		return ;
	shell->cmd_list = create_list(tokens, fd, shell);
	if (!shell->cmd_list)
		return ;
	if (fd->flag_out || fd->flag_append)
		shell->fd_out = fd->fd_out[0];
	if (execute_built_in(shell, tokens, fd))
		return ;
	id = fork();
	if (id == 0)
		execute_child(shell, tokens, fd);
	signal(SIGINT, SIG_IGN);
	get_exit_status(id, shell);
	cleanup_execute_command(shell, fd);
	// fprintf(stderr, "exit status = %i\n", shell->exit_status);
}

void	execute(t_shell *shell, t_token *tokens)
{
	(void)shell;
	if (!tokens->content)
		return ;
	if (how_many_pipes(tokens) > 0)
	{
		execute_multiple(tokens, shell);
		signal_handler();
	}
	else
	{
		execute_command(tokens, shell);
		signal_handler();
	}
}
