/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhatahet <qhatahet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 11:05:04 by qhatahet          #+#    #+#             */
/*   Updated: 2025/05/24 16:21:31 by qhatahet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_heredoc(t_shell *shell, t_fds *fd)
{
	if (shell && shell->head)
		free_tokenizer(shell->head);
	if (shell->cmd_list)
		ft_free_2d(shell->cmd_list);
	if (shell->env)
		free_env(shell->env);
	if (shell && shell->enviroment)
		ft_free_2d(shell->enviroment);
	if (shell)
		free(shell);
	if (fd && fd->temp)
		free(fd->temp);
	if (fd)
		free(fd);
}

static void	child(t_fds *fds, t_shell *shell, char *delimiter, int j)
{
	if (delimiter[0] == '\'' || delimiter[0] == '\"')
		delimiter = remove_qoutes(delimiter, shell);
	while (1)
	{
		fds->text = readline("> ");
		if (!fds->text)
		{
			close(fds->fd_in[j]);
			heredoc_ctrl_d(fds->text, delimiter, shell, fds);
		}
		if (!shell->expand_flag)
			fds->text = expand_heredoc(fds->text, shell);
		if (!ft_strcmp(fds->text, delimiter))
		{
			free(fds->text);
			free(delimiter);
			close(fds->fd_in[j]);
			free_heredoc(shell, fds);
			exit(0);
		}
		write(fds->fd_in[j], fds->text, ft_strlen(fds->text));
		write(fds->fd_in[j], "\n", 1);
		free(fds->text);
	}
	free(delimiter);
}

static void	parent(pid_t pid, t_shell *shell, struct sigaction original_sa)
{
	int	status;

	waitpid(pid, &status, 0);
	sigaction(SIGINT, &original_sa, NULL);
	if (WIFEXITED(status))
	{
		shell->exit_status = WEXITSTATUS(status);
		if (shell->exit_status == 128 + SIGINT)
			return ;
	}
	else if (WIFSIGNALED(status))
	{
		shell->exit_status = 128 + WTERMSIG(status);
		return ;
	}
}

static void	handle_heredoc_loop(t_shell *shell, char **lst, t_fds *fds,
		struct sigaction original_sa)
{
	fds->index_i = 0;
	fds->index_j = 0;
	while (lst[fds->index_i])
	{
		if (!ft_strcmp(lst[fds->index_i], "<<"))
		{
			fds->index_i++;
			open_file(fds, fds->index_j);
			fds->pid = fork();
			if (fds->pid == 0)
			{
				fds->delimiter = ft_strdup(lst[fds->index_i]);
				if (!fds->delimiter)
					exit(1);
				handle_heredoc_child();
				child(fds, shell, fds->delimiter, fds->index_j);
				close(fds->fd_in[0]);
			}
			else if (fds->pid > 0)
				parent(fds->pid, shell, original_sa);
			close(fds->fd_in[0]);
			fds->index_j++;
		}
		fds->index_i++;
	}
}

void	open_heredoc(t_shell *shell, char **lst, t_fds *fds)
{
	struct sigaction	sa;
	struct sigaction	original_sa;

	fds->temp = ft_strdup(".temp");
	fds->flag_expand = 1;
	sigaction(SIGINT, NULL, &original_sa);
	sa.sa_handler = SIG_IGN;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	handle_heredoc_loop(shell, lst, fds, original_sa);
	fds->flag_heredoc = 1;
}
