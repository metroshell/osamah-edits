/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalananz <oalananz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 17:38:09 by oalananz          #+#    #+#             */
/*   Updated: 2025/05/21 17:52:28 by oalananz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_there_redirectin(t_token *tokens)
{
    t_token *temp = tokens;

    int i = 0;
    while(temp->content[i])
    {
        if(temp->type[i] == REDIRECTIN)
            return(1);
        i++;
    }
    return (0);
}

int is_there_redirectout(t_token *tokens)
{
    t_token *temp = tokens;

    int i = 0;
    while(temp->content[i])
    {
        if(temp->type[i] == REDIRECTOUT)
            return(1);
        i++;
    }
    return (0);
}

int is_there_append(t_token *tokens)
{
    t_token *temp = tokens;

    int i = 0;
    while(temp->content[i])
    {
        if(temp->type[i] == APPEND)
            return(1);
        i++;
    }
    return (0);
}