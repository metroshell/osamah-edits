/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_outjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalananz <oalananz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 06:02:02 by oalananz          #+#    #+#             */
/*   Updated: 2025/03/30 06:10:13 by oalananz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_dollar(t_token *token, t_expand *expand, char *temp, int *i)
{
	int	j;

	j = 0;
	if (expand->output)
	{
		expand->inner++;
		if (ft_isalpha(token->content[expand->outer][expand->inner]))
			expand->inner += expand->var_length - 1;
		else if (ft_isdigit(token->content[expand->outer][expand->inner]))
			expand->inner++;
		while (expand->output[j])
			temp[(*i)++] = expand->output[j++];
		return (1);
	}
	return (0);
}

void	ft_outjoin(t_token *token, t_expand *expand)
{
	char	*temp;
	int		i;

	temp = malloc(expand->inner + ft_strlen(expand->output) + 1);
	if (!temp)
		return ;
	expand->inner = 0;
	i = 0;
	while (token->content[expand->outer][expand->inner])
	{
		if (token->content[expand->outer][expand->inner] == '$')
		{
			if (handle_dollar(token, expand, temp, &i))
				continue ;
		}
		temp[i++] = token->content[expand->outer][expand->inner++];
	}
	temp[i] = '\0';
	free(token->content[expand->outer]);
	token->content[expand->outer] = temp;
}
