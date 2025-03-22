/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalananz <oalananz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 00:04:10 by oalananz          #+#    #+#             */
/*   Updated: 2025/03/22 23:24:24 by oalananz         ###   ########.fr       */
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

void	detect_texts(t_parser *parser, t_token *temp)
{
	temp->type[parser->index] = TEXT;
	parser->texts_counter++;
	parser->texts_flag = 0;
}

void	ft_parser(t_token *head, t_shell *shell)
{
	t_parser	*parser;

	parser = ft_calloc(1, sizeof(t_parser));
	if (!parser)
		exit(EXIT_FAILURE);
	get_paths(shell);
	while (head)
	{
		while (head->content[parser->index])
		{
			parser->filename_flag = 0;
			parser->texts_flag = 0;
			detect_command(parser, head, shell->paths);
			detect_arguments(parser, head);
			detect_heredoc(parser, head);
			detect_redirect(parser, head);
			if (parser->filename_flag)
				detect_filename(parser, head);
			else if (!parser->texts_flag)
				detect_texts(parser, head);
			parser->index++;
		}
		parser->index = 0;
		head = head->next;
	}
}
