/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_multiple.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalananz <oalananz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 17:28:42 by oalananz          #+#    #+#             */
/*   Updated: 2025/05/23 19:17:32 by oalananz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	child(struct sigaction sa, int i, t_shell *shell, t_token *temp)
{
	int		fd;
	char	*exit_heredoc;

	fd = 0;
	sa.sa_handler = heredoc_signal_handler;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	exit_heredoc = ft_strdup(temp->content[i + 1]);
	if (fd)
		close(fd);
	fd = open_heredocs(shell, exit_heredoc, temp->heredoc_file);
	exit(0);
}

static void	parent(pid_t pid, struct sigaction original_sa, t_shell *shell)
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

static void	process(t_token *temp, struct sigaction original_sa,
		struct sigaction sa, t_shell *shell)
{
	int		i;
	pid_t	pid;

	i = 0;
	while (temp->content[i])
	{
		if (temp->type[i] == HEREDOC)
		{
			pid = fork();
			if (pid == 0)
				child(sa, i, shell, temp);
			else if (pid > 0)
				parent(pid, original_sa, shell);
		}
		i++;
	}
}

void	heredoc_handle(t_token *tokens, t_shell *shell)
{
	t_token				*temp;
	struct sigaction	sa;
	struct sigaction	original_sa;

	temp = tokens;
	sigaction(SIGINT, NULL, &original_sa);
	sa.sa_handler = SIG_IGN;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	while (temp)
	{
		process(temp, original_sa, sa, shell);
		temp = temp->next;
	}
}
