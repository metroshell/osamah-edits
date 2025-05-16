/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalananz <oalananz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 14:34:44 by oalananz          #+#    #+#             */
/*   Updated: 2025/05/17 02:13:27 by oalananz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exit_zero(t_token *token, t_shell *shell)
{
	free_shell(shell);
	free_tokenizer(token);
	exit(0);
}

void	ft_exit(t_token *token, t_shell *shell)
{
	char	*tmp;
	char	*temp;
	int		status;

	if (token->content[1])
	{
		if (valid_arg(token->content[1]) == -1)
		{
			tmp = ft_strjoin("ARSSH:  exit: ", token->content[1]);
			temp = ft_strjoin(tmp, ": numeric argument required\n");
			free(tmp);
			write(2, temp, ft_strlen(temp));
			free(temp);
			free_shell(shell);
			free_tokenizer(token);
			exit(2);
		}
		status = ft_atoi(token->content[1]);
		free_shell(shell);
		free_tokenizer(token);
		exit(status);
	}
	else
		exit_zero(token, shell);
}
