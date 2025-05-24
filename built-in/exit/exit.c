/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhatahet <qhatahet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 14:34:44 by oalananz          #+#    #+#             */
/*   Updated: 2025/05/24 14:28:57 by qhatahet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exit_zero(t_token *token, t_shell *shell)
{
	int	x;

	(void)token;
	x = shell->exit_status;
	free_shell(shell);
	free_tokenizer(shell->head);
	exit(x);
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
			free_tokenizer(shell->head);
			exit(2);
		}
		status = ft_atoi(token->content[1]);
		free_shell(shell);
		free_tokenizer(shell->head);
		exit(status);
	}
	else
		exit_zero(token, shell);
}
