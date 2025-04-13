/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalananz <oalananz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 21:54:42 by oalananz          #+#    #+#             */
/*   Updated: 2025/04/13 19:14:22 by oalananz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	delete_node(t_shell *shell, t_export *export)
{
    t_env	*current;
    t_env	*prev;

    current = shell->env;
    prev = NULL;
    while (current)
    {
        if (ft_strcmp(current->variable, export->variable) == 0)
        {
            if (prev == NULL)
                shell->env = current->next;
            else
                prev->next = current->next;
            free(current->content);
            free(current->variable);
            free(current);
            return ;
        }
        prev = current;
        current = current->next;
    }
}

void	unset_command(t_shell *shell, t_token *token)
{
	shell->temp_index = 1;
	while (token->content[shell->temp_index])
		shell->temp_index++;
	t_export *export;
	
	export = ft_calloc(1,sizeof(t_export));
	if(!export)
		return ;
	int i = 1;
	while(token->content[i])
	{
		export->flag = 0;
		export->variable = ft_strdup(token->content[i]);
		scan_env(shell,export);
		if(export->flag == 1)
			delete_node(shell, export);
		else if(export->flag == -1)
			return ;
		i++;
	}
}