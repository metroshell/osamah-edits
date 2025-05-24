/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhatahet <qhatahet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 20:47:51 by oalananz          #+#    #+#             */
/*   Updated: 2025/05/24 14:31:40 by qhatahet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ctrl_d(t_shell *shell)
{
	int	x;

	printf("exit\n");
	if (shell->env)
		free_env(shell->env);
	x = shell->exit_status;
	if (shell)
	{
		free(shell);
		shell = NULL;
	}
	exit(x);
}

void	free_loop(t_shell *shell, t_token *tokens)
{
	if (tokens)
		execute(shell, tokens);
	if (shell && shell->prompt)
		free(shell->prompt);
	if (tokens)
		free_tokenizer(tokens);
}
