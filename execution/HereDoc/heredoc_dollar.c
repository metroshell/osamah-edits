/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_dollar.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalananz <oalananz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 18:01:29 by oalananz          #+#    #+#             */
/*   Updated: 2025/05/23 18:44:26 by oalananz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	expand_alpha_norm(char *text, t_shell *shell)
{
	int	count;

	count = 0;
	while (text[shell->temp_index] && (ft_isalpha(text[shell->temp_index])
			|| text[shell->temp_index] == '_'))
	{
		shell->temp_index++;
		count++;
	}
	return (count);
}

static void	expand_alpha(t_shell *shell, char *text, char *temp, int *i)
{
	t_env	*current;
	int		count;
	int		index;

	count = expand_alpha_norm(text, shell);
	shell->variable = ft_substr(text, shell->temp_index - count, count);
	current = shell->env;
	while (current)
	{
		if (ft_strcmp(current->variable, shell->variable) == 0)
		{
			index = 0;
			while (current->content[index])
				temp[(*i)++] = current->content[index++];
			free(shell->variable);
			shell->variable = NULL;
			return ;
		}
		current = current->next;
	}
	free(shell->variable);
	shell->variable = NULL;
}

void	dollar(char *text, t_shell *shell, char *temp, int *i)
{
	int		count;
	char	*digit;

	count = 0;
	shell->temp_index++;
	if (text[shell->temp_index] == ' ' || text[shell->temp_index] == '\0')
		temp[(*i)++] = '$';
	if (ft_isdigit(text[shell->temp_index]) && text[shell->temp_index] == '0')
	{
		digit = ft_strdup("ARSSH");
		shell->temp_index++;
		while (digit[count])
			temp[(*i)++] = digit[count++];
		free(digit);
	}
	else if (ft_isdigit(text[shell->temp_index]))
		shell->temp_index++;
	else
		expand_alpha(shell, text, temp, i);
}

static int	length_norm(t_shell *shell, char *text)
{
	int		count;
	t_env	*current;

	count = 0;
	while (text[shell->temp_index] && (ft_isalpha(text[shell->temp_index])
			|| text[shell->temp_index] == '_'))
	{
		shell->temp_index++;
		count++;
	}
	shell->variable = ft_substr(text, shell->temp_index - count, count);
	current = shell->env;
	while (current)
	{
		if (ft_strcmp(current->variable, shell->variable) == 0)
		{
			count = ft_strlen(current->content);
			return (count);
		}
		current = current->next;
	}
	return (count);
}

int	length(char *text, t_shell *shell)
{
	int	count;

	count = 0;
	shell->temp_index++;
	if (text[shell->temp_index] == ' ' || text[shell->temp_index] == '\0')
		count++;
	else if (text[shell->temp_index] == '0')
	{
		shell->temp_index++;
		count += 5;
	}
	else if (ft_isdigit(text[shell->temp_index]))
		shell->temp_index++;
	else
		count = length_norm(shell, text);
	return (count);
}
