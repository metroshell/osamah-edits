/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalananz <oalananz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 00:04:10 by oalananz          #+#    #+#             */
/*   Updated: 2025/03/28 22:30:23 by oalananz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// add backslash to the path
void	add_backslash(t_shell *shell)
{
	int		i;
	int		j;
	char	*temp;
	int		l;

	i = 0;
	j = 0;
	temp = NULL;
	while (shell->paths[i])
	{
		l = ft_strlen(shell->paths[i]);
		temp = malloc(l + 2);
		while (shell->paths[i][j])
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
void	get_paths(t_shell *shell)
{
    t_env	*current;

    current = shell->env; // Start from the head of the linked list
    while (current)
    {
        if (ft_strncmp(current->variable, "PATH", 4) == 0 && current->content)
        {
            shell->paths = ft_split(current->content, ':');
            if (!shell->paths)
                exit(EXIT_FAILURE);
            add_backslash(shell);
            break;
        }
        current = current->next;
    }
}

void	ft_parser(t_token *head,t_parser *parser, t_shell *shell)
{
	get_paths(shell);
	while (head)
	{
		while (head->content[parser->index])
		{
			parser->filename_flag = 0;
			detect_command(parser, head, shell->paths);
			detect_arguments(parser, head);
			detect_redirect(parser, head);
			detect_heredoc(parser, head);
			parser->index++;
			if (parser->filename_flag )
				detect_filename(parser, head);
		}
		parser->index = 0;
		head = head->next;
	}
}
