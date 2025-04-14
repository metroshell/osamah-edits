/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalananz <oalananz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 14:10:23 by oalananz          #+#    #+#             */
/*   Updated: 2025/04/14 16:33:13 by oalananz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	get_length(t_token *token, t_expand *expand)
// {
// 	while ((ft_isalpha(token->content[expand->outer][expand->inner])
// 		|| token->content[expand->outer][expand->inner] == '_')
// 		&& token->content[expand->outer][expand->inner])
// 	{
// 		expand->var_length++;
// 		expand->inner++;
// 	}
// 	expand->inner -= expand->var_length;
// }

// void	copy_var(t_token *token, t_expand *expand)
// {
// 	int	i;

// 	get_length(token, expand);
// 	expand->variable = malloc(expand->var_length + 1);
// 	if (!expand->variable)
// 		return ;
// 	i = 0;
// 	while (token->content[expand->outer][expand->inner]
// 		&& (ft_isalpha(token->content[expand->outer][expand->inner])
// 		|| token->content[expand->outer][expand->inner] == '_'))
// 	{
// 		expand->variable[i] = token->content[expand->outer][expand->inner];
// 		i++;
// 		expand->inner++;
// 	}
// 	expand->variable[i] = '\0';
// }

void	check_cmd(t_token *token, t_expand *expand, char **paths)
{
	char	*command_path;
	int		i;

	command_path = NULL;
	i = 0;
	if(ft_strcmp(token->content[expand->outer],"bash") == 0)
		return ;
	while (paths[i])
	{
		command_path = ft_strjoin(paths[i], token->content[expand->outer]);
		if (!command_path)
			exit(EXIT_FAILURE);
		if (!access(command_path, X_OK))
		{
			token->type[expand->outer] = COMMAND;
			break ;
		}
		i++;
	}
	if (command_path)
		free(command_path);
}