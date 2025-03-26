/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalananz <oalananz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 21:31:16 by oalananz          #+#    #+#             */
/*   Updated: 2025/02/22 21:31:26 by oalananz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


//env command
void    env_command(t_shell *shell)
{
    int i = 0;
    if(ft_strncmp(shell->prompt , "env", 3) == 0)
    {
        while (shell->environment[i])
        {
            ft_printf("%s\n", shell->environment[i]);
            i++;
        }
    }
}

//enviroment copy
void    env_copy(t_shell *shell, char **env)
{
    int i = 0;
    while (env[i] != NULL)
        i++;
    shell->env_counter = i;
    shell->environment = ft_calloc(i + 1, sizeof(char *));
    if (!shell->environment)
        exit(EXIT_FAILURE);
    i = 0;
    while (env[i] != NULL)
    {
        shell->environment[i] = ft_strdup(env[i]);
        if (!shell->environment[i])
            exit(EXIT_FAILURE);
        i++;
    }
    shell->environment[i] = NULL;
}