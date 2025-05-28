/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qais <qais@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 20:41:23 by oalananz          #+#    #+#             */
/*   Updated: 2025/05/26 12:32:34 by qais             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_executor_norm(t_shell *shell, t_token *token, t_fds *fd)
{
	if (ft_strcmp(token->content[0], "unset") == 0)
	{
		unset_command(shell, token);
		return (1);
	}
	else if (ft_strcmp(token->content[0], "pwd") == 0)
	{
		ft_pwd();
		return (1);
	}
	else if (ft_strcmp(token->content[0], "cd") == 0)
	{
		ft_cd(shell, token);
		return (1);
	}
	else if (ft_strcmp(token->content[0], "exit") == 0)
	{
		ft_exit(token, shell, fd);
		return (1);
	}
	return (0);
}

int	ft_executor(t_shell *shell, t_token *token, t_fds *fd)
{
	if (token->content[0])
	{
		if (ft_strcmp(token->content[0], "env") == 0)
		{
			print_env(shell->env);
			return (1);
		}
		else if (ft_strcmp(token->content[0], "echo") == 0)
		{
			echo_command(shell, token);
			return (1);
		}
		else if (ft_strcmp(token->content[0], "export") == 0)
		{
			export_command(shell, token);
			return (1);
		}
		else
			return (ft_executor_norm(shell, token, fd));
	}
	return (0);
}

void	start_minishell(t_shell *shell)
{
	t_token		*tokens;
	t_parser	*parser;

	add_history(shell->prompt);
	tokens = tokenizer(shell);
	if (!tokens)
	{
		free(shell->prompt);
		return ;
	}
	parser = ft_calloc(1, sizeof(t_parser));
	if (!parser)
		exit(EXIT_FAILURE);
	if (tokens)
	{
		ft_parser(tokens, parser, shell);
		if (shell->paths)
		{
			ft_free_2d(shell->paths);
			shell->paths = NULL;
		}
		free(parser);
		ft_expander(shell, tokens);
	}
	free_loop(shell, tokens);
}

void	init_minishell(t_shell *shell)
{
	while (1)
	{
		shell->prompt = readline("\033[93mArab Spring 🐣🐥 -> \033[0m");
		if (!shell->prompt)
			ctrl_d(shell);
		if (!ft_strcmp(shell->prompt, ""))
			continue ;
		else if (shell->prompt)
			start_minishell(shell);
		else
			exit(0);
		if (shell && shell->enviroment)
		{
			ft_free_2d(shell->enviroment);
			shell->enviroment = NULL;
		}
	}
}

// take the arguments and the enviroment
int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;

	(void)argv;
	if (argc > 1)
		return (1);
	shell = ft_calloc(1, sizeof(t_shell));
	if (!shell)
		exit(EXIT_FAILURE);
	if (envp[0] == NULL)
		shell->env = NULL;
	else
		env_copy(shell, envp);
	signal_handler();
	init_minishell(shell);
	if (shell->env)
	{
		free_env(shell->env);
		free(shell->env);
		shell->env = NULL;
	}
	free(shell->prompt);
	free(shell);
	rl_clear_history();
	return (0);
}
