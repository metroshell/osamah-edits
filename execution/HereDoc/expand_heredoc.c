/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalananz <oalananz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 18:11:20 by oalananz          #+#    #+#             */
/*   Updated: 2025/05/23 18:11:29 by oalananz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	string_len(char *text, t_shell *shell)
{
	int	size;

	shell->temp_index = 0;
	size = 0;
	if (!text)
		return (0);
	while (shell->temp_index < (int)ft_strlen(text) && text[shell->temp_index])
	{
		if (text[shell->temp_index] == '$')
		{
			size += length(text, shell);
			free(shell->variable);
			shell->variable = NULL;
		}
		else
		{
			shell->temp_index++;
			size++;
		}
	}
	shell->temp_index = 0;
	return (size);
}

char	*expand_heredoc(char *text, t_shell *shell)
{
	char	*temp;
	int		i;
	int		x;

	x = string_len(text, shell);
	if ((int)ft_strlen(text) == x)
		return (text);
	temp = malloc(x + 1);
	if (!temp)
		exit(130);
	i = 0;
	while (text[shell->temp_index])
	{
		if (text[shell->temp_index] == '$')
			dollar(text, shell, temp, &i);
		else
			temp[i++] = text[shell->temp_index++];
	}
	temp[i] = '\0';
	free(text);
	return (temp);
}
