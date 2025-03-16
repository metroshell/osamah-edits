/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qais <qais@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 20:41:23 by oalananz          #+#    #+#             */
/*   Updated: 2025/03/15 15:30:26 by qais             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// enviroment copy
void	env_copy(t_shell *shell, char **env)
{
	int	i;

	i = 0;
	while (env[i] != NULL)
		i++;
	shell->env_counter = i;
	shell->environment = ft_calloc(i + 1, sizeof(char *));
	if (!shell->environment)
		exit(EXIT_FAILURE);
	i = 0;
	while (env[i] != NULL)
	{
		shell->environment[i] = ft_strdup(env[i]);
		if (!shell->environment[i])
			exit(EXIT_FAILURE);
		i++;
	}
	shell->environment[i] = NULL;
}

// test the tokenizer
void	print_tokens(t_token **arg)
{
	t_token	**tmp;
	int		i;

	i = 0;
	tmp = arg;
	while (*tmp)
	{
		i = 0;
		while ((*tmp)->content[i] != NULL)
		{
			printf("argv[%i] = %s\n", i, (*tmp)->content[i]);
			i++;
		}
		*tmp = (*tmp)->next;
		if (*tmp)
			printf("------->%c\n", '|');
	}
}

// take the arguments and the enviroment
int	main(int argc, char **argv, char **env)
{
	t_shell	*shell;
	t_token	*tokens;

	(void)argv;
	if (argc > 1)
	{
		printf("Error\nwrong number of arguments\n");
		return (1);
	}
	shell = ft_calloc(1, sizeof(t_shell));
	if (!shell)
		exit(EXIT_FAILURE);
	env_copy(shell, env);
	while (1)
	{
		shell->prompt = readline("Arab Spring 🐣🐥 -> ");
		if (shell->prompt)
		{
			add_history(shell->prompt);
			tokens = tokenizer(shell);
			print_tokens(&tokens);
		}
		else
			exit(EXIT_FAILURE);
	}
	free(shell->prompt);
	return (0);
}
