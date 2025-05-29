/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirect.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qais <qais@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 19:39:22 by oalananz          #+#    #+#             */
/*   Updated: 2025/05/29 10:28:46 by qais             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	dup_in(t_fds *fd, int *j)
{
	if (fd->flag_in)
	{
		fd->fd_in[(*j)] = open(fd->in_file, O_RDONLY);
		free(fd->in_file);
		dup2(fd->fd_in[(*j)], STDIN_FILENO);
		close(fd->fd_in[(*j)]);
	}
	else if (!fd->flag_in)
		close(fd->saved_in);
}

void	dup_out(t_fds *fd, int *j)
{
	int	i;

	i = (*j);
	if (fd->flag_out)
	{
		fd->fd_out[i] = open(fd->out_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		free(fd->out_file);
		dup2(fd->fd_out[i], STDOUT_FILENO);
		close(fd->fd_out[i]);
	}
	else if (fd->flag_append)
	{
		fd->fd_out[i] = open(fd->out_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		free(fd->out_file);
		dup2(fd->fd_out[i], STDOUT_FILENO);
		close(fd->fd_out[i]);
	}
	else if (!fd->flag_out || !fd->flag_append)
		close(fd->saved_out);
}

int	redirect(char **lst, t_shell *shell, t_fds *fd)
{
	int	i;

	i = 0;
	while (lst[i])
	{
		if (!ft_strcmp(lst[i++], "<<"))
		{
			open_heredoc(shell, lst, fd);
			printf("hello there\n");
			break ;
		}
	}
	i = -1;
	while (lst[++i])
	{
		if (!ft_strcmp(lst[i], ">") || !ft_strcmp(lst[i], ">>")
			|| !ft_strcmp(lst[i], "<"))
		{
			if (open_redirect_out(fd, lst) != 1)
				return (0);
			if (open_redirect_in(fd, lst) != 1)
				return (0);
			break ;
		}
	}
	return (1);
}

int	redirections(char **lst, int *i)
{
	if (!ft_strcmp(lst[(*i)], ">") || !ft_strcmp(lst[(*i)], "<")
		|| !ft_strcmp(lst[(*i)], ">>") || !ft_strcmp(lst[(*i)], "<<"))
		return (0);
	return (1);
}

int	open_appendfiles(t_fds *fd, char **lst, int *i, int *j)
{
	(*i)++;
	fd->out_file = ft_strdup(lst[(*i)]);
	fd->fd_out[(*j)] = open(fd->out_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd->fd_out[(*j)] < 0)
	{
		write(1, "arssh: ", 7);
		perror(lst[(*i)]);
		free(fd->out_file);
		close(fd->saved_out);
		return (fd->fd_out[(*j)]);
	}
	close(fd->fd_out[(*j)]);
	fd->flag_append = 1;
	(*i)++;
	if (lst[(*i)] && (!ft_strcmp(lst[(*i)], ">>")
		|| !ft_strcmp(lst[(*i)], ">")))
	{
		free(fd->out_file);
		fd->out_file = NULL;
	}
	return (1);
}
