/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalananz <oalananz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 14:10:23 by oalananz          #+#    #+#             */
/*   Updated: 2025/03/30 06:08:46 by oalananz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	get_length(t_token *token, t_expand *expand)
{
	while (ft_isalpha(token->content[expand->outer][expand->inner])
		&& token->content[expand->outer][expand->inner])
	{
		expand->var_length++;
		expand->inner++;
	}
	expand->inner -= expand->var_length;
	expand->var_length++;
}

void	copy_var(t_token *token, t_expand *expand)
{
	int	i;

	get_length(token, expand);
	expand->variable = malloc(expand->var_length + 1);
	if (!expand->variable)
		return ;
	i = 0;
	while (ft_isalpha(token->content[expand->outer][expand->inner])
		&& token->content[expand->outer][expand->inner])
	{
		expand->variable[i] = token->content[expand->outer][expand->inner];
		i++;
		expand->inner++;
	}
	expand->variable[i] = '\0';
	expand->inner -= i - 1;
}

void	check_env(t_shell *shell, t_expand *expand)
{
	t_env	*current;

	current = shell->env;
	while (current)
	{
		if (ft_strcmp(current->variable, expand->variable) == 0)
		{
			expand->output = ft_strdup(current->content);
			return ;
		}
		current = current->next;
	}
	expand->output = NULL;
}
