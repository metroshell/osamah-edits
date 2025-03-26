/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalananz <oalananz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 16:06:40 by oalananz          #+#    #+#             */
/*   Updated: 2025/03/26 18:14:04 by oalananz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    get_length(t_token *token, t_expand *expand)
{
   while(token->content[expand->outer_index][expand->inner_index] != ' ' 
        && token->content[expand->outer_index][expand->inner_index] != '\"' 
        && token->content[expand->outer_index][expand->inner_index] 
        && token->content[expand->outer_index][expand->inner_index] != '+')
    {
        expand->var_length++;
        expand->inner_index++;
    }
    expand->var_length++;
    expand->inner_index -= expand->var_length;
}

void    copy_var(t_token *token, t_expand *expand)
{
    get_length(token,expand);
    expand->variable = malloc(expand->var_length + 1);
    if (!expand->variable)
        return ;
    int i = 0;
    while(token->content[expand->outer_index][expand->inner_index] != ' ' 
        && token->content[expand->outer_index][expand->inner_index] != '\"' 
        && token->content[expand->outer_index][expand->inner_index] 
        && token->content[expand->outer_index][expand->inner_index] != '+')
    {
        expand->variable[i] = token->content[expand->outer_index][expand->inner_index];
        i++;
        expand->inner_index++;
    }
    expand->variable[i] = '=';
    i++;
    expand->variable[i] = '\0';
    expand->inner_index - i - 1;
}

void    check_env(t_shell *shell, t_token *token, t_expand *expand)
{
    int	i;

    token->content[expand->outer_index] = ft_calloc(1,sizeof(char));
    if (!token->content[expand->outer_index])
        exit(EXIT_FAILURE);
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
        check_env(shell,token,expand);
        char *temp;

        temp = malloc(expand->inner_index + ft_strlen(expand->output) + 1);
        if(!temp)
            return ;
        expand->inner_index = 0;
        int i = 0;
        while(token->content[expand->outer_index][expand->inner_index])
        {
            if(token->content[expand->outer_index][expand->inner_index] == '$')
            {
                expand->inner_index += 1 + expand->var_length;
                int j = 0;
                while(expand->output[j])
                    temp[i++] = expand->output[j++];
            }
            temp[i++] = token->content[expand->outer_index][expand->inner_index++];
        }
        temp[i] = '\0';
        free(token->content[expand->outer_index]);
        token->content[expand->outer_index] = temp;
    }
    // if(ft_isdigit(token->content[expand->outer_index][expand->inner_index]) && token->content[expand->outer_index][expand->inner_index] == '0')
    // {
    //     char *temp = all->output;
    //     all->output = ft_strjoin(all->output ,"bash");
    //     free(temp);
    //     all->index_out += 4;
    //     all->index++;
    // }
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
        expand->quote_flag = 1;
}

void    quote_remover(t_token *token,t_expand *expand)
{
    count_quotes(token,expand);
    if(expand->quote_flag == 1)
        return ;
    expand->inner_index = 0;
    char *temp;
    int i;

    i = 0;
    temp = malloc(ft_strlen(token->content[expand->outer_index]) - expand->quotes_count + 1);
    if(!temp)
        return ;
    while(token->content[expand->outer_index][expand->inner_index])
    {
        if(token->content[expand->outer_index][expand->inner_index] == expand->quote)
            expand->inner_index++;
        temp[i++] = token->content[expand->outer_index][expand->inner_index++];
    }
    temp[i] = '\0';
    free(token->content[expand->outer_index]);
    token->content[expand->outer_index] = temp;
}

void    ft_expander(t_shell *shell, t_token *token)
{
    t_expand *expand;

    expand = ft_calloc(1,sizeof(t_expand));
    if(!expand)
        return ;
    (void) shell;
    while(token)
    {
        while(token->content[expand->outer_index])
        {
            if(token->type[expand->outer_index] == TEXT)
            {
                while(token->content[expand->outer_index][expand->inner_index])
                {
                    if(token->content[expand->outer_index][expand->inner_index] == '\"'
                        || token->content[expand->outer_index][expand->inner_index] == '\'')
                        quote_remover(token,expand);
                    if(!expand->single_qoute && token->content[expand->outer_index][expand->inner_index] == '$')
                        expand_dollar(shell,token,expand);
                    expand->inner_index++;
                }
                expand->inner_index = 0;
            }   
            expand->outer_index++;
        }
        token = token->next;
    }
}
    