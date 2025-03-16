/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qais <qais@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 13:59:09 by oalananz          #+#    #+#             */
/*   Updated: 2025/03/16 05:47:07 by qais             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	check_quotes(t_shell *shell)
// {
// 	int	i;
// 	int	dub;
// 	int	single;

// 	i = 0;
// 	dub = 0;
// 	single = 0;
// 	while (shell->prompt[i])
// 	{
// 		if (shell->prompt[i] == '\"')
// 			dub++;
// 		else if (shell->prompt[i] == '\'')
// 			single++;
// 		i++;
// 	}
// 	if (dub % 2 != 0 || single % 2 != 0)
// 	{
// 		printf("arssh: unclosed qoutes\n");
// 		shell->token_flag = 1;
// 	}
// }

void	check_quotes(t_shell *shell)
{
	int	i;
	int	quote;

	i = 0;
	quote = 0;
	while (shell->prompt[i])
	{
		if (shell->prompt[i] == '\"' && shell->prompt[i])
		{
			quote = 1;
			i++;
			while (shell->prompt[i] != '\"' && shell->prompt[i])
				i++;
			if (shell->prompt[i] == '\"' && shell->prompt[i])
			{
				quote = 2;
				i++;
			}
		}
		else if (shell->prompt[i] == '\'' && shell->prompt[i])
		{
			quote = 1;
			i++;
			while (shell->prompt[i] != '\'' && shell->prompt[i])
				i++;
			if (shell->prompt[i] == '\'' && shell->prompt[i])
			{
				quote = 2;
				i++;
			}
		}
		else
			i++;
	}
	if (quote % 2 != 0)
	{
		printf("arssh: unclosed qoutes\n");
		shell->token_flag = 1;
	}
}

int	check_pipe(t_shell *shell)
{
	int	i;

	i = shell->prompt_index;
	if (shell->prompt[i] == '|')
		i++;
	if (shell->prompt[i] == '\0')
		return (-1);
	while (shell->prompt[i] && shell->prompt[i] == ' ')
		i++;
	if (shell->prompt[i] == '\0')
		return (-1);
	return (1);
}

void	check_prompt_4(t_shell *shell)
{
	if (ft_strncmp(&shell->prompt[shell->prompt_index], "||||", 4) == 0)//error number 1
	{
		printf("arssh: syntax error near unexpected token `||'\n");
		shell->token_flag = 1;
		shell->prompt_index += 4;
	}
	else if (ft_strncmp(&shell->prompt[shell->prompt_index], "|||", 3) == 0)//error number 1
	{
		printf("arssh: syntax error near unexpected token `|'\n");
		shell->token_flag = 1;
		shell->prompt_index += 3;
	}
	else if (ft_strncmp(&shell->prompt[shell->prompt_index], "||", 2) == 0)//error number 130
	{
		printf("arssh: syntax error near unexpected token `||'\n");
		shell->token_flag = 1;
		shell->prompt_index += 2;
	}
	else if (ft_strncmp(&shell->prompt[shell->prompt_index], "|", 1) == 0
	&& check_pipe(shell) < 0)//error number 130
	{
		printf("arssh: unclosed pipe\n");
		shell->token_flag = 1;
		shell->prompt_index += 1;
	}
}

void	check_prompt_1(t_shell *shell)
{
	if (ft_strncmp(&shell->prompt[shell->prompt_index], "<<<", 3) == 0)
	{
		printf("arssh: syntax error near unexpected token `<<'\n");
		shell->token_flag = 1;
		shell->prompt_index += 3;
	}
	else if (ft_strncmp(&shell->prompt[shell->prompt_index], ">>>>", 4) == 0)
	{
		printf("arssh: syntax error near unexpected token `>>'\n");
		shell->token_flag = 1;
		shell->prompt_index += 4;
	}
	else if (ft_strncmp(&shell->prompt[shell->prompt_index], ">>>", 3) == 0)
	{
		printf("arssh: syntax error near unexpected token `>'\n");
		shell->token_flag = 1;
		shell->prompt_index += 3;
	}
	else if (ft_strncmp(&shell->prompt[shell->prompt_index], "><", 2) == 0)
	{
		printf("arssh: syntax error near unexpected token `<'\n");
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
	while (shell->prompt[shell->prompt_index] == ' ')
		shell->prompt_index++;
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
	else if (shell->prompt[shell->prompt_index]
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

void	check_errors(t_shell *shell)
{
	check_quotes(shell);
	check_prompt_1(shell);
	check_prompt_4(shell);	
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
	while (shell->prompt_index < (int)ft_strlen(shell->prompt)
		&& shell->prompt[shell->prompt_index])
	{
		check_errors(shell);
		if (shell->token_flag)
			return (NULL);
		check_prompt_2(shell, &token);
		check_prompt_3(shell, &token);
		while (shell->prompt_index < (int)ft_strlen(shell->prompt)
			&& shell->prompt[shell->prompt_index] == ' ')
			shell->prompt_index++;
	}
	token->content[shell->token_index] = NULL;
	return (head);
}
