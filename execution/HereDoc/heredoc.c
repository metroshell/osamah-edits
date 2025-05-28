/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qais <qais@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 11:05:04 by qhatahet          #+#    #+#             */
/*   Updated: 2025/05/28 11:11:43 by qais             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_heredoc(t_shell *shell, t_fds *fd)
{
	// if (shell && shell->head)
	// 	free_tokenizer(shell->head);
	free_shell(shell);
	if (fd && fd->temp)
		free(fd->temp);
	if (fd)
		free(fd);
}

static void	child(t_fds *fds, t_shell *shell, char *delimiter, int j)
{
	// signal(SIGINT, heredoc_signal_handler);  // Custom handler to catch Ctrl+C
	// signal(SIGQUIT, SIG_IGN); 
	if (delimiter && (delimiter[0] == '\'' || delimiter[0] == '\"'))
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

void	heredoc_parent(pid_t pid, t_shell *shell, struct sigaction original_sa)
{
	int status;
	struct sigaction sa_ignore;

	// Ignore SIGINT temporarily while waiting
	sa_ignore.sa_handler = SIG_IGN;
	sa_ignore.sa_flags = 0;
	sigemptyset(&sa_ignore.sa_mask);
	sigaction(SIGINT, &sa_ignore, NULL);
	waitpid(pid, &status, 0);
	// Restore original signal handling
	sigaction(SIGINT, &original_sa, NULL);

	if (WIFEXITED(status))
	{
		shell->exit_status = WEXITSTATUS(status);
		if (shell->exit_status == 128 + SIGINT)
			shell->heredoc_interrupted = 1;
	}
	else if (WIFSIGNALED(status))
	{
		shell->exit_status = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGINT)
			shell->heredoc_interrupted = 1;
	}
}

static void	handle_heredoc_loop(t_shell *shell, char **lst, t_fds *fds,
		struct sigaction original_sa)
{
	fds->index_i = 0;
	fds->index_j = 0;
	shell->heredoc_interrupted = 0;
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
				heredoc_parent(fds->pid, shell, original_sa);
			close(fds->fd_in[0]);
			if (shell->heredoc_interrupted)
			{
				close(fds->fd_in[0]);
				break;
			}
			fds->index_j++;
		}
		fds->index_i++;
	}
}

void	open_heredoc(t_shell *shell, char **lst, t_fds *fds)
{
	struct sigaction	original_sa;

	fds->temp = ft_strdup(".temp");
	fds->flag_expand = 1;
	sigaction(SIGINT, NULL, &original_sa);
	handle_heredoc_loop(shell, lst, fds, original_sa);
	fds->flag_heredoc = 1;
	sigaction(SIGINT, &original_sa, NULL);
}
