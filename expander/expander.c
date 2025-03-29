/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalananz <oalananz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 16:06:40 by oalananz          #+#    #+#             */
/*   Updated: 2025/03/29 04:37:32 by oalananz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



void    ft_outjoin(t_token *token,t_expand *expand)
{
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
            int j = 0;
            if(expand->output)
            {
                expand->inner_index++;
                if(ft_isalpha(token->content[expand->outer_index][expand->inner_index]))
                    expand->inner_index += expand->var_length - 1;
                else if(ft_isdigit(token->content[expand->outer_index][expand->inner_index]))
                    expand->inner_index++;
                while(expand->output[j])
                    temp[i++] = expand->output[j++];
                
            }
        }
        temp[i++] = token->content[expand->outer_index][expand->inner_index++];
    }
    temp[i] = '\0';
    free(token->content[expand->outer_index]);
    token->content[expand->outer_index] = temp;
}

void    check_cmd(t_token *token, t_expand *expand, char **paths)
{
    char	*command_path;
	int		i;

	command_path = NULL;
	i = 0;
	while (paths[i])
	{
		command_path = ft_strjoin(paths[i], token->content[expand->outer_index]);
		if (!command_path)
			exit(EXIT_FAILURE);
		if (!access(command_path, X_OK))
		{
			token->type[expand->outer_index] = COMMAND;
			break ;
		}
		i++;
	}
	if (command_path)
		free(command_path);
}


void    quote_remover(t_token *token,t_expand *expand)
{
    //int tmp;
    
    expand->quote_flag = 1;
    //tmp = expand->inner_index;
    count_quotes(token,expand);
    if(expand->quote_flag == 2)
        return ;
    expand->inner_index = 0;
    char *temp;
    int i;

    i = 0;
    temp = malloc(ft_strlen(token->content[expand->outer_index]) - expand->quotes_count + 1);
    if(!temp)
        return ;
    while(token->content[expand->outer_index][i])
    {
        if(token->content[expand->outer_index][expand->inner_index] == expand->quote)
            expand->inner_index++;
        else
            temp[i++] = token->content[expand->outer_index][expand->inner_index++];
    }
    temp[i] = '\0';
    free(token->content[expand->outer_index]);
    token->content[expand->outer_index] = temp;
    // expand->inner_index = tmp + 1;
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
        expand->outer_index = 0;
        while(token->content[expand->outer_index])
        {
            if(token->type[expand->outer_index] == TEXT)
            {
                while(token->content[expand->outer_index][expand->inner_index])
                {
                    if(token->content[expand->outer_index][expand->inner_index] == '\"'
                        || token->content[expand->outer_index][expand->inner_index] == '\'')
                        quote_remover(token,expand);
                    if(expand->quote_flag)
                        break ;
                    expand->inner_index++;
                }
                if(token->content[expand->outer_index][0] != '\0')
                    check_cmd(token,expand,shell->paths);
                expand->inner_index = 0;
                while(token->content[expand->outer_index][expand->inner_index])
                {
                    if(!expand->single_qoute && token->content[expand->outer_index][expand->inner_index] == '$')
                        expand_dollar(shell,token,expand);
                    expand->inner_index++;
                }
            }
            expand->inner_index = 0;
            expand->quote_flag = 0;
            expand->single_qoute = 0;
            expand->outer_index++;
        }
        token = token->next;
    }
    free(expand);
}
    