/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qais <qais@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 16:03:27 by oalananz          #+#    #+#             */
/*   Updated: 2025/03/15 16:52:21 by qais             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*create_new_node(t_shell *shell)
{
	t_token	*new_node;

	new_node = malloc(sizeof(t_token));
	if (!new_node)
		exit(EXIT_FAILURE);
	shell->counter = 0;
	shell->temp_index = shell->prompt_index;
	tokenizer_size(shell);
	new_node->content = ft_calloc(shell->counter + 1, sizeof(char *));
	if (!new_node->content)
		exit(EXIT_FAILURE);
	new_node->next = NULL;
	return (new_node);
}

void	copy_quotes(t_shell *shell, t_token *token, char detect)
{
	int	start;
	int	len;

	shell->qoute_flag = 0;
	start = shell->prompt_index;
	len = 1;
	shell->prompt_index++;
	while (shell->prompt[shell->prompt_index])
	{
		if (shell->prompt[shell->prompt_index] == detect)
			shell->qoute_flag = 1;
		if (shell->qoute_flag == 1 && (shell->prompt[shell->prompt_index] == ' '
				|| shell->prompt[shell->prompt_index] == '|'
				|| shell->prompt[shell->prompt_index] == '>'
				|| shell->prompt[shell->prompt_index] == '<'))
			break ;
		len++;
		shell->prompt_index++;
	}
	token->content[shell->token_index] = ft_substr(shell->prompt, start, len);
	shell->token_index++;
	while (shell->prompt[shell->prompt_index] == ' ')
		shell->prompt_index++;
}

void	copy_norm(t_shell *shell)
{
	if (shell->prompt[shell->prompt_index] == '\"')
	{
		shell->prompt_index++;
		while (shell->prompt[shell->prompt_index] != '\"'
			&& shell->prompt[shell->prompt_index])
			shell->prompt_index++;
	}
	else if (shell->prompt[shell->prompt_index] == '\'')
	{
		shell->prompt_index++;
		while (shell->prompt[shell->prompt_index] != '\''
			&& shell->prompt[shell->prompt_index])
			shell->prompt_index++;
	}
}

void	copy(t_shell *shell, t_token *token)
{
	int	start;
	int	len;

	start = shell->prompt_index;
	while (shell->prompt[shell->prompt_index]
		&& shell->prompt[shell->prompt_index] != ' '
		&& shell->prompt[shell->prompt_index] != '|'
		&& shell->prompt[shell->prompt_index] != '>'
		&& shell->prompt[shell->prompt_index] != '<')
	{
		copy_norm(shell);
		shell->prompt_index++;
	}
	len = shell->prompt_index - start;
	token->content[shell->token_index] = ft_substr(shell->prompt, start, len);
	shell->token_index++;
	while (shell->prompt[shell->prompt_index] == ' ')
		shell->prompt_index++;
}
