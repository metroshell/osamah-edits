/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   requirements.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalananz <oalananz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 19:37:44 by oalananz          #+#    #+#             */
/*   Updated: 2025/05/23 19:38:10 by oalananz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_env(t_env *env)
{
	t_env	*temp;
	char	**envp;
	char	*tmp;
	int		i;

	temp = env;
	envp = ft_calloc((env_count(env) + 1), sizeof(char *));
	i = 0;
	while (temp)
	{
		tmp = ft_strjoin(temp->variable, "=");
		envp[i] = ft_strjoin(tmp, temp->content);
		free(tmp);
		temp = temp->next;
		i++;
	}
	envp[i] = NULL;
	return (envp);
}

int	count_content(t_token *tokens)
{
	int		i;
	t_token	*temp;

	temp = tokens;
	i = 0;
	while (temp->content[i])
		i++;
	return (i);
}
