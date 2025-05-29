/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhatahet <qhatahet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 14:34:44 by oalananz          #+#    #+#             */
/*   Updated: 2025/05/29 14:13:00 by qhatahet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exit_zero(t_shell *shell, t_fds *fd)
{
	int	x;

	x = shell->exit_status;
	if (shell && shell->head)
		free_tokenizer(shell->head);
	if (shell)
	{
		free_shell(shell);
		free(shell);
	}
	if (fd)
	{
		free(fd);
		fd = NULL;
	}
	exit(x);
}

static void	clean_up_and_exit(t_shell *shell, t_fds *fd, int status)
{
	if (fd)
	{
		free(fd);
		fd = NULL;
	}
	if (shell && shell->head)
		free_tokenizer(shell->head);
	if (shell)
	{
		free_shell(shell);
		free(shell);
	}
	exit(status);
}

void	ft_exit(t_token *token, t_shell *shell, t_fds *fd)
{
	char	*tmp;
	char	*temp;

	if (token->content[1])
	{
		if (valid_arg(token->content[1]) == -1)
		{
			tmp = ft_strjoin("ARSSH:  exit: ", token->content[1]);
			temp = ft_strjoin(tmp, ": numeric argument required\n");
			free(tmp);
			write(2, temp, ft_strlen(temp));
			free(temp);
			clean_up_and_exit(shell, fd, 2);
		}
		if (token->content[2])
		{
			write (2, "exit\n", 5);
			write (2, "ARSSH: exit: too many arguments\n", 33);
			shell->exit_status = 1;
			return ;
		}
		clean_up_and_exit(shell, fd, ft_atoi(token->content[1]));
	}
	else
		exit_zero(shell, fd);
}
