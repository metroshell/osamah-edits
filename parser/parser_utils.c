/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalananz <oalananz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 00:16:39 by oalananz          #+#    #+#             */
/*   Updated: 2025/03/19 15:59:50 by oalananz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// add backslash to the path
void    add_backslash(t_shell *shell)
{
    int i = 0;
    int j = 0;
    char *temp = NULL;

    while(shell->paths[i])
    {
        int l = ft_strlen(shell->paths[i]);
        temp = malloc(l + 2);
        while(shell->paths[i][j])
        {
            temp[j] = shell->paths[i][j];
            j++;
        }
        temp[j] = '/';
        temp[j + 1] = '\0';
        free(shell->paths[i]);
        shell->paths[i] = temp;
        j = 0;
        i++;
    }
}

// take a copy of paths to detect commands 
void    get_paths(t_shell *shell)
{
	int	i;

	i = 0;
    
	while (shell->environment[i])
	{
		if (ft_strncmp(shell->environment[i], "PATH=", 5) == 0)
		{
			shell->paths = ft_split(shell->environment[i] + 5, ':');
			if (!shell->paths)
				exit(EXIT_FAILURE);
            add_backslash(shell);
		}
		i++;
	}
}
