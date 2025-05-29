/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_list.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qais <qais@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 19:30:22 by oalananz          #+#    #+#             */
/*   Updated: 2025/05/29 10:30:26 by qais             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	list_utils(t_token *temp, int i, char **lst)
{
	int	j;

	j = 0;
	if (temp->type[i] != REDIRECTIN && temp->type[i] != REDIRECTOUT
		&& temp->type[i] != APPEND && temp->type[i] != HEREDOC && temp->type[i])
	{
		lst[j] = ft_strdup(temp->content[i]);
		if (!lst)
			return (-1);
		free(temp->content[i]);
		temp->content[i] = ft_strdup("ta3rees");
		j++;
	}
	return (j);
}

static void	list_write(t_token *temp, int j, char **lst)
{
	int	i;

	i = 0;
	while (temp->content[i] && i < count_content(temp))
	{
		if (!ft_strcmp(temp->content[i], "ta3rees"))
			i++;
		else if (temp->content[i])
		{
			lst[j] = ft_strdup(temp->content[i]);
			if (!lst[i])
				return ;
			i++;
			j++;
		}
	}
	lst[j] = NULL;
}

void	rearrange_list_redirect(t_token *tokens, t_shell *shell)
{
	t_token	*temp;
	int		i;

	i = 0;
	temp = tokens;
	shell->cmd_list = malloc((count_content(tokens) + 1) * sizeof(char *));
	if (!shell->cmd_list)
		return ;
	while (temp->content[i])
	{
		if (temp->type[i] == REDIRECTIN || temp->type[i] == REDIRECTOUT
			|| temp->type[i] == APPEND || temp->type[i] == HEREDOC)
			i += 2;
		else
			break ;
	}
	list_write(temp, list_utils(temp, i, shell->cmd_list), shell->cmd_list);
}

void	rearrange_list(t_token *tokens, t_shell *shell)
{
	t_token	*temp;
	int		i;

	temp = tokens;
	i = 0;
	shell->cmd_list = malloc((count_content(tokens) + 1) * sizeof(char *));
	if (!shell->cmd_list)
		return ;
	while (temp->content[i])
	{
		if (temp->content[i])
			shell->cmd_list[i] = ft_strdup(temp->content[i]);
		else
			shell->cmd_list[i] = NULL;
		if (!shell->cmd_list[i])
			return ;
		i++;
	}
	shell->cmd_list[i] = NULL;
}

char	**create_list(t_token *tokens, t_fds *fd, t_shell *shell)
{
	char	**redirect_lst;

	if (redirect_first_arg(tokens))
		rearrange_list_redirect(tokens, shell);
	else
		rearrange_list(tokens, shell);
	if (is_there_redirect(tokens))
	{
		redirect_lst = open_files(shell->cmd_list, tokens, fd, shell);
		if (!redirect_lst)
		{
			if (shell->cmd_list)
				ft_free_2d(shell->cmd_list);
			if (fd->saved_in)
				close(fd->saved_in);
			if (fd->saved_out)
				close(fd->saved_out);
			free(fd);
			return (NULL);
		}
		return (redirect_lst);
	}
	else
		return (shell->cmd_list);
}
