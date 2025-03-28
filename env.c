/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalananz <oalananz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 21:31:16 by oalananz          #+#    #+#             */
/*   Updated: 2025/03/28 22:28:15 by oalananz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


//env command
// void    env_command(t_shell *shell)
// {
//     int i = 0;
//     if(ft_strncmp(shell->prompt , "env", 3) == 0)
//     {
//         while (shell->environment[i])
//         {
//             ft_printf("%s\n", shell->environment[i]);
//             i++;
//         }
//     }
// }

void    copy_variable(t_shell *shell,t_env *temp,char **env)
{
    int end;

    end = 0;
    while(env[shell->temp_index][end] != '=' && env[shell->temp_index][end])
        end++;
    temp->variable=ft_substr(env[shell->temp_index],0,end);
    if(!temp->variable)
        return ;
}

void    copy_content(t_shell *shell,t_env *temp,char **env)
{
    int end;
    int start;

    end = 0;
    start = 0;
    while(env[shell->temp_index][start] != '=' && env[shell->temp_index][start])
        start++;
    while(env[shell->temp_index][end])
        end++;
    start++;
    temp->content=ft_substr(env[shell->temp_index],start,end);
    if(!temp->content)
        return ;
}

t_env   *create_env_node()
{
    t_env   *new_node;

    new_node = malloc(sizeof(t_env));
	if (!new_node)
		exit(EXIT_FAILURE);
    new_node->next = NULL;
    return(new_node);
}

//enviroment copy
void    env_copy(t_shell *shell, char **env)
{
    shell->env = create_env_node();
    if (!shell->env)
        return ;
    t_env *temp = shell->env;
    shell->temp_index = 0;
    while (env[shell->temp_index])
    {
        copy_variable(shell, temp, env);
        copy_content(shell, temp, env);
        if (env[shell->temp_index + 1])
        {
            temp->next = create_env_node();
            temp = temp->next;
        }
        shell->temp_index++;
    }
    shell->temp_index = 0;
}