/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalananz <oalananz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 14:10:23 by oalananz          #+#    #+#             */
/*   Updated: 2025/03/27 15:29:21 by oalananz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    get_length(t_token *token, t_expand *expand)
{
    while(ft_isalpha(token->content[expand->outer_index][expand->inner_index])
        && token->content[expand->outer_index][expand->inner_index] )
    {
        expand->var_length++;
        expand->inner_index++;
    }
    expand->inner_index -= expand->var_length;
    expand->var_length++;
}

void    copy_var(t_token *token, t_expand *expand)
{
    get_length(token,expand);
    expand->variable = malloc(expand->var_length + 1);
    if (!expand->variable)
        return ;
    int i = 0;
    while(ft_isalpha(token->content[expand->outer_index][expand->inner_index])
        && token->content[expand->outer_index][expand->inner_index] )
    {
        expand->variable[i] = token->content[expand->outer_index][expand->inner_index];
        i++;
        expand->inner_index++;
    }
    expand->variable[i] = '=';
    i++;
    expand->variable[i] = '\0';
    expand->inner_index -= i - 1;
}

void    check_env(t_shell *shell, t_expand *expand)
{
    int	i;

    i = 0;
	while (shell->environment[i])
	{
		if (ft_strncmp(shell->environment[i], expand->variable, ft_strlen(expand->variable)) == 0)
		{
            expand->output = ft_strdup(&shell->environment[i][ft_strlen(expand->variable)]);
            return ;
		}
		i++;
	}
}


void    expand_dollar(t_shell *shell,t_token *token, t_expand *expand)
{
    expand->inner_index++;
    if(ft_isalpha(token->content[expand->outer_index][expand->inner_index]))
    {
        copy_var(token,expand);
        check_env(shell,expand);
    }
    if(ft_isdigit(token->content[expand->outer_index][expand->inner_index]) 
        && token->content[expand->outer_index][expand->inner_index] == '0')
        expand->output = ft_strdup("bash");
    ft_outjoin(token,expand);
}

void    count_quotes(t_token *token,t_expand *expand)
{
    expand->quote = token->content[expand->outer_index][expand->inner_index];
    if(expand->quote == '\'')
        expand->single_qoute = 1;
    expand->quotes_count++;
    expand->inner_index++;
    while(token->content[expand->outer_index][expand->inner_index])
    {
        if(token->content[expand->outer_index][expand->inner_index] == expand->quote)
            expand->quotes_count++;
        expand->inner_index++;
    }
    if(expand->quotes_count % 2 != 0)
        expand->quote_flag = 2;
}