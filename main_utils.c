/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalananz <oalananz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 20:47:51 by oalananz          #+#    #+#             */
/*   Updated: 2025/05/23 20:48:05 by oalananz         ###   ########.fr       */
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
	free(shell);
	exit(x);
}

void	free_loop(t_shell *shell, t_token *tokens, t_parser *parser)
{
	if (tokens)
		execute(shell, tokens, parser);
	if (shell->prompt)
		free(shell->prompt);
	if (parser)
		free(parser);
	if (tokens)
		free_tokenizer(tokens);
}
