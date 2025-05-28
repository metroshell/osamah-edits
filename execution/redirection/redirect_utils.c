/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qais <qais@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 17:38:09 by oalananz          #+#    #+#             */
/*   Updated: 2025/05/27 07:01:37 by qais             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_there_redirectin(t_token *tokens)
{
	t_token	*temp;
	int		i;

	temp = tokens;
	i = 0;
	while (temp->content[i])
	{
		if (temp->type[i] == REDIRECTIN)
			return (1);
		i++;
	}
	return (0);
}

int	is_there_redirectout(t_token *tokens)
{
	t_token	*temp;
	int		i;

	temp = tokens;
	i = 0;
	while (temp->content[i])
	{
		if (temp->type[i] == REDIRECTOUT)
			return (1);
		i++;
	}
	return (0);
}

int	is_there_append(t_token *tokens)
{
	t_token	*temp;
	int		i;

	temp = tokens;
	i = 0;
	while (temp->content[i])
	{
		if (temp->type[i] == APPEND)
			return (1);
		i++;
	}
	return (0);
}

void	restore_in_out(t_fds *fd)
{
	if (fd->flag_out || fd->flag_append)
	{
		dup2(fd->saved_out, STDOUT_FILENO);
		if (fd->saved_out > 0)
			close(fd->saved_out);
	}
	if (fd->flag_in)
	{
		dup2(fd->saved_in, STDIN_FILENO);
		if (fd->saved_in > 0)
			close(fd->saved_in);
	}
	if (fd->temp)
	{
		if (fd->fd_in[0] != 0)
			close (fd->fd_in[0]);
		free(fd->temp);
	}
	if (fd)
		free(fd);
}
