/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_files.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qais <qais@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 19:39:59 by oalananz          #+#    #+#             */
/*   Updated: 2025/05/27 07:01:31 by qais             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	open_redirect_out(t_fds *fd, char **lst)
{
	int	i;
	int	j;

	fd->saved_out = dup(STDOUT_FILENO);
	fd->flag_append = 0;
	fd->flag_out = 0;
	i = 0;
	j = 0;
	while (lst[i])
	{
		if (!ft_strcmp(lst[i], ">"))
		{
			if (open_outfiles(fd, lst, &i, &j) < 0)
				return (fd->fd_out[j]);
		}
		else if (!ft_strcmp(lst[i], ">>"))
		{
			if (open_appendfiles(fd, lst, &i, &j) < 0)
				return (fd->fd_out[j]);
		}
		else
			i++;
	}
	dup_out(fd, &j);
	return (1);
}

int	open_redirect_in(t_fds *fd, char **lst)
{
	int	i;
	int	j;

	fd->saved_in = dup(STDIN_FILENO);
	fd->flag_in = 0;
	i = 0;
	j = 0;
	while (lst[i])
	{
		if (!ft_strcmp(lst[i], "<"))
		{
			if (open_infiles(fd, lst, &i, &j) < 0)
				return (fd->fd_in[j]);
		}
		else
			i++;
	}
	dup_in(fd, &j);
	return (1);
}

int	open_outfiles(t_fds *fd, char **lst, int *i, int *j)
{
	(*i)++;
	fd->out_file = ft_strdup(lst[(*i)]);
	fd->fd_out[(*j)] = open(fd->out_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd->fd_out[(*j)] < 0)
	{
		write(1, "arssh: ", 7);
		perror(lst[(*i)]);
		free(fd->out_file);
		close(fd->saved_out);
		return (fd->fd_out[(*j)]);
	}
	close(fd->fd_out[(*j)]);
	fd->flag_out = 1;
	(*i)++;
	if (lst[(*i)]&& (!ft_strcmp(lst[(*i)], ">")
		|| !ft_strcmp(lst[(*i)], ">>")))
	{
		free(fd->out_file);
		fd->out_file = NULL;
	}
	return (1);
}

int	open_infiles(t_fds *fd, char **lst, int *i, int *j)
{
	(*i)++;
	fd->in_file = ft_strdup(lst[(*i)]);
	fd->fd_in[(*j)] = open(fd->in_file, O_RDONLY);
	if (fd->fd_in[(*j)] < 0)
	{
		write(1, "arssh: ", 7);
		perror(lst[(*i)]);
		free(fd->in_file);
		close(fd->saved_in);
		return (fd->fd_in[(*j)]);
	}
	fd->flag_in = 1;
	(*i)++;
	close(fd->fd_in[(*j)]);
	if (lst[(*i)] && !ft_strcmp(lst[(*i)], "<"))
	{
		free(fd->in_file);
		fd->in_file = NULL;
	}
	return (1);
}

char	**open_files(char **lst, t_token *tokens, t_fds *fd, t_shell *shell)
{
	int		i;
	int		j;
	char	**new_list;

	i = 0;
	j = 0;
	if (!redirect(lst, shell, fd))
		return (NULL);
	new_list = ft_calloc((count_content(tokens) + 1), sizeof(char *));
	while (lst[i])
	{
		if (!redirections(lst, &i))
			i += 2;
		else if (lst[i])
		{
			new_list[j] = ft_strdup(lst[i]);
			if (!new_list[j])
				return (NULL);
			i++;
			j++;
		}
	}
	ft_free_2d(lst);
	new_list[j] = NULL;
	return (new_list);
}
