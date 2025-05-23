/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalananz <oalananz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 19:13:01 by oalananz          #+#    #+#             */
/*   Updated: 2025/05/23 19:19:42 by oalananz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_heredoc_child(void)
{
	struct sigaction	sa;

	sa.sa_handler = heredoc_signal_handler;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
}

void	open_file(t_fds *fds, int j)
{
	fds->fd_in[j] = open(fds->temp, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fds->fd_in[j] < 0)
		exit(1);
}

int	check_text(char *text, t_shell *shell)
{
	t_env	*temp;

	temp = shell->env;
	while (temp)
	{
		if (!ft_strcmp(text, temp->variable))
			return (1);
		temp = temp->next;
	}
	return (0);
}

void	heredoc_signal_handler(int sig)
{
	(void)sig;
	rl_redisplay();
	write(2, "\n", 1);
	rl_on_new_line();
	if (!access(".temp", F_OK))
		unlink(".temp");
	exit(128 + SIGINT);
}
