/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qais <qais@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 19:46:19 by oalananz          #+#    #+#             */
/*   Updated: 2025/05/27 07:01:17 by qais             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	list_redirect_2(t_token *temp, char **lst, int *i, int *j)
{
	if (temp->type[*i] == HEREDOC || temp->type[*i] == REDIRECTIN
		|| temp->type[*i] == REDIRECTOUT || temp->type[*i] == APPEND
		|| temp->type[*i] == ENDOFFILE || temp->type[*i] == FILENAME)
		(*i)++;
	else if (temp->type[*i] == COMMAND || temp->type[*i] == ARGUMENT
		|| temp->type[*i] == TEXT)
	{
		lst[*j] = ft_strdup(temp->content[*i]);
		(*i)++;
		(*j)++;
	}
}

char	**list_redirect(t_token *tokens)
{
	t_token	*temp;
	char	**lst;
	int		i;
	int		j;

	i = 0;
	temp = tokens;
	lst = malloc((counter(tokens) + 1) * sizeof(char *));
	if (!lst)
		return (NULL);
	j = 0;
	while (temp->content[i])
		list_redirect_2(temp, lst, &i, &j);
	if (is_there_command(temp) && is_there_heredoc(temp))
	{
		if (temp->heredoc_file)
			lst[j++] = ft_strdup(temp->heredoc_file);
	}
	lst[j] = NULL;
	return (lst);
}

void	copy_command_line_2(int *i, t_token *temp, char **list, int *j)
{
	if (temp->type[*i] == HEREDOC || temp->type[*i] == REDIRECTIN
		|| temp->type[*i] == REDIRECTOUT || temp->type[*i] == APPEND
		|| temp->type[*i] == ENDOFFILE || temp->type[*i] == FILENAME)
		(*i)++;
	else if (temp->type[*i] == COMMAND || temp->type[*i] == ARGUMENT
		|| temp->type[*i] == TEXT)
	{
		list[*j] = ft_strdup(temp->content[*i]);
		if (!list[*j])
			return ;
		(*j)++;
		(*i)++;
	}
}

char	**copy_command_line(t_token *tokens)
{
	char	**list;
	t_token	*temp;
	int		i;
	int		j;

	temp = tokens;
	i = 0;
	j = 0;
	list = malloc((counter(tokens) + 1) * sizeof(char *));
	if (!list)
		return (NULL);
	while (temp->content[i])
		copy_command_line_2(&i, temp, list, &j);
	if (is_there_command(temp) && is_there_heredoc(temp))
	{
		if (temp->heredoc_file)
			list[j++] = ft_strdup(temp->heredoc_file);
	}
	list[j] = NULL;
	return (list);
}

char	**list(t_token *tokens)
{
	char	**list;

	if (redirect_first_arg(tokens))
		list = list_redirect(tokens);
	else
		list = copy_command_line(tokens);
	return (list);
}
