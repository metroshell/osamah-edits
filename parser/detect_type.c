/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   detect_type.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalananz <oalananz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 00:16:39 by oalananz          #+#    #+#             */
/*   Updated: 2025/03/28 21:38:18 by oalananz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	detect_command(t_parser *parser, t_token *temp, char **paths)
{
	char	*command_path;
	int		i;

	command_path = NULL;
	i = 0;
	while (paths[i])
	{
		command_path = ft_strjoin(paths[i], temp->content[parser->index]);
		if (!command_path)
			exit(EXIT_FAILURE);
		if (!access(command_path, X_OK))
		{
			temp->type[parser->index] = COMMAND;
			parser->commands_counter++;
			break ;
		}
		i++;
	}
	if (command_path)
		free(command_path);
}

void	detect_arguments(t_parser *parser, t_token *temp)
{
	if (temp->content[parser->index][0] == '-'
		&& ft_isalpha(temp->content[parser->index][1]))
	{
		temp->type[parser->index] = ARGUMENT;
		parser->arguments_counter++;
	}
}

void	detect_heredoc(t_parser *parser, t_token *temp)
{
	if (ft_strncmp(temp->content[parser->index], "<<", 2) == 0)
	{
		temp->type[parser->index] = HEREDOC;
		parser->heredocs_counter++;
		parser->filename_flag = 1;
	}
	else if (ft_strncmp(temp->content[parser->index], ">>", 2) == 0)
	{
		temp->type[parser->index] = APPEND;
		parser->append_counter++;
		parser->filename_flag = 1;
	}
}

void	detect_redirect(t_parser *parser, t_token *temp)
{
	if (*temp->content[parser->index] == '>'
		|| *temp->content[parser->index] == '<')
	{
		temp->type[parser->index] = REDIRECT;
		parser->redirect_counter++;
		parser->filename_flag = 1;
	}
}

void	detect_filename(t_parser *parser, t_token *temp)
{
	temp->type[parser->index] = FILENAME;
	parser->filename_counter++;
}
