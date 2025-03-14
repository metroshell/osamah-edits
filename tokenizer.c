/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qais <qais@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 13:59:09 by oalananz          #+#    #+#             */
/*   Updated: 2025/03/14 23:38:56 by qais             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_prompt_1(t_shell *shell)
{
	if (ft_strncmp(&shell->prompt[shell->prompt_index], "<<<", 3) == 0)
	{
		printf("bash: syntax error near unexpected token `<<'\n");
		shell->token_flag = 1;
		shell->prompt_index += 3;
	}
	else if (ft_strncmp(&shell->prompt[shell->prompt_index], ">>>>", 4) == 0)
	{
		printf("bash: syntax error near unexpected token `>>'\n");
		shell->token_flag = 1;
		shell->prompt_index += 4;
	}
	else if (ft_strncmp(&shell->prompt[shell->prompt_index], ">>>", 3) == 0)
	{
		printf("bash: syntax error near unexpected token `>'\n");
		shell->token_flag = 1;
		shell->prompt_index += 3;
	}
	else if (ft_strncmp(&shell->prompt[shell->prompt_index], "><", 2) == 0)
	{
		printf("bash: syntax error near unexpected token `<'\n");
		shell->token_flag = 1;
		shell->prompt_index += 2;
	}
}

void	check_prompt_2(t_shell *shell, t_token **token)
{
	if (ft_strncmp(&shell->prompt[shell->prompt_index], "<<", 2) == 0)
	{
		(*token)->content[shell->token_index++] = ft_strdup("<<");
		shell->prompt_index += 2;
	}
	if (ft_strncmp(&shell->prompt[shell->prompt_index], ">>", 2) == 0)
	{
		(*token)->content[shell->token_index++] = ft_strdup(">>");
		shell->prompt_index += 2;
	}
	if (shell->prompt[shell->prompt_index] == '<')
	{
		(*token)->content[shell->token_index++] = ft_strdup("<");
		shell->prompt_index++;
	}
	if (shell->prompt[shell->prompt_index] == '>')
	{
		(*token)->content[shell->token_index++] = ft_strdup(">");
		shell->prompt_index++;
	}
}

void	check_prompt_3(t_shell *shell, t_token **token)
{
	if (shell->prompt[shell->prompt_index] == '|')
	{
		(*token)->content[shell->token_index] = NULL;
		shell->prompt_index++;
		(*token)->next = create_new_node(shell);
		*token = (*token)->next;
		shell->token_index = 0;
	}
	else if (ft_isascii(shell->prompt[shell->prompt_index])
		|| (shell->prompt[shell->prompt_index] == '-'
			&& ft_isalpha(shell->prompt[shell->prompt_index + 1])))
		copy(shell, *token);
	else if (shell->prompt[shell->prompt_index] == '\"')
		copy_quotes(shell, *token, '\"');
	else if (shell->prompt[shell->prompt_index] == '\'')
		copy_quotes(shell, *token, '\'');
	else
		shell->prompt_index++;
}

t_token	*tokenizer(t_shell *shell)
{
	t_token	*head;
	t_token	*token;

	shell->prompt_index = 0;
	shell->token_index = 0;
	shell->token_flag = 0;
	shell->counter = 0;
	head = create_new_node(shell);
	token = head;
	while (shell->prompt[shell->prompt_index] == ' ')
		shell->prompt_index++;
	while (shell->prompt[shell->prompt_index])
	{
		check_prompt_1(shell);
		if (shell->token_flag)
			return (NULL);
		check_prompt_2(shell, &token);
		check_prompt_3(shell, &token);
		while (shell->prompt[shell->prompt_index] == ' ')
			shell->prompt_index++;
	}
	token->content[shell->token_index] = NULL;
	return (head);
}
