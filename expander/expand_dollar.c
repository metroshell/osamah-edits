/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_dollar.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalananz <oalananz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 06:02:02 by oalananz          #+#    #+#             */
/*   Updated: 2025/04/05 12:36:33 by oalananz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_dollar(t_shell *shell, t_token *token, t_expand *expand, char *temp, int *i)
{
	int	j;

	expand->inner++;
	if (ft_isalpha(token->content[expand->outer][expand->inner])
		|| token->content[expand->outer][expand->inner] == '_')
		{
			copy_var(token, expand);
			check_env(shell, expand);
		}
	else if(ft_isdigit(token->content[expand->outer][expand->inner]) 
		&& token->content[expand->outer][expand->inner] == '0')
		{
			expand->output = ft_strdup("bash");
			expand->inner++;
		}
	else if (ft_isdigit(token->content[expand->outer][expand->inner]))
		expand->inner++;
	if (expand->output)
	{
		j = 0;
		expand->flag = 1;
		while (expand->output[j])
		{
			temp[(*i)] = expand->output[j];
			(*i)++;
			j++;
		}
		free(expand->output);
		expand->output = NULL;
	}
}

void	expand_dollar(t_shell *shell,t_token *token, t_expand *expand)
{
	char	*temp;
	int		i;

	i = (int)ft_strlen(token->content[expand->outer]) + (int)ft_strlen(expand->output) - expand->var_length + 1;
	temp = ft_calloc(1,1);
	expand->inner = 0;
	i = 0;
	expand->flag = 0;
	while (token->content[expand->outer][expand->inner])
	{
		if (token->content[expand->outer][expand->inner] == '$' && expand->flag != 1 )
		{
			handle_dollar(shell, token, expand, temp, &i);
			expand->flag = 1;
		}
		temp[i] = token->content[expand->outer][expand->inner];
		i++;
		expand->inner++;
	}
	temp[i] = '\0';
	free(token->content[expand->outer]);
	token->content[expand->outer] = temp;
}
