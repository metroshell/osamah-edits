/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_list.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalananz <oalananz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 19:30:22 by oalananz          #+#    #+#             */
/*   Updated: 2025/05/23 20:21:31 by oalananz         ###   ########.fr       */
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

char	**rearrange_list_redirect(t_token *tokens)
{
	t_token	*temp;
	char	**lst;
	int		i;

	i = 0;
	temp = tokens;
	lst = malloc((count_content(tokens) + 1) * sizeof(char *));
	if (!lst)
		return (NULL);
	while (temp->content[i])
	{
		if (temp->type[i] == REDIRECTIN || temp->type[i] == REDIRECTOUT
			|| temp->type[i] == APPEND || temp->type[i] == HEREDOC)
			i += 2;
		else
			break ;
	}
	list_write(temp, list_utils(temp, i, lst), lst);
	return (lst);
}

char	**rearrange_list(t_token *tokens)
{
	char	**list;
	t_token	*temp;
	int		i;

	temp = tokens;
	i = 0;
	list = malloc((count_content(tokens) + 1) * sizeof(char *));
	if (!list)
		return (NULL);
	while (temp->content[i])
	{
		if (temp->content[i])
			list[i] = ft_strdup(temp->content[i]);
		else
			list[i] = NULL;
		if (!list[i])
			return (NULL);
		i++;
	}
	list[i] = NULL;
	i = 0;
	return (list);
}

char	**create_list(t_token *tokens, t_fds *fd, t_shell *shell)
{
	char	**lst;
	char	**redirect_lst;

	if (redirect_first_arg(tokens))
		lst = rearrange_list_redirect(tokens);
	else
		lst = rearrange_list(tokens);
	if (is_there_redirect(tokens))
	{
		redirect_lst = open_files(lst, tokens, fd, shell);
		if (!redirect_lst)
		{
			if (lst)
				ft_free_2d(lst);
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
		return (lst);
}
