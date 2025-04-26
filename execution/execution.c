/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalananz <oalananz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 14:22:32 by qhatahet          #+#    #+#             */
/*   Updated: 2025/04/26 16:39:06 by oalananz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	how_many_pipes(t_token *tokens)
{
	t_token *temp;
	int	i;

	i = 0;
	temp = tokens;
	while(temp)
	{
		temp = temp->next;
		i++;
	}
	return (i - 1);
}

int	is_there_redirect(t_token *tokens)
{
	t_token	*temp;
	int		i;

	i = 0;
	temp = tokens;
	if (temp->type[i] == REDIRECTIN || temp->type[i] == REDIRECTOUT
		|| temp->type[i] == HEREDOC || temp->type[i] == APPEND)
		return (1);
	return (0);
}

int	is_there_command(t_token *tokens)
{
	t_token	*temp;
	int			i;

	temp = tokens;
	i = 0;
	while (temp->content[i])
	{
		if (temp->type[i] == COMMAND)
			return (1);
		i++;
	}
	return (0);
}

int	count_content(t_token *tokens)
{
	int	i;
	t_token	*temp;

	temp = tokens;
	i = 0;
	while(temp->content[i])
		i++;
	return (i);
}

int	count_env(t_env *env)
{
	int			i;
	t_env	*temp;

	i = 0;
	temp = env;
	while(temp != NULL)
	{
		temp = temp->next;
		i++;
	}
	return (i);
}

char	**get_env(t_env *env)
{
	t_env	*temp;
	char	**envp;
	char	*str;
	int			i;
	char		*tmp;

	temp = env;
	envp = ft_calloc((count_env(env) + 1), sizeof(char *));
	i = 0;
	while (temp)
	{
		tmp = ft_strjoin(temp->variable, "=");
		str = ft_strjoin(tmp, temp->content);
		envp[i] = ft_strdup(str);
		free(str);
		free(tmp);
		temp = temp->next;
		i++;
	}
	envp[i] = NULL;
	return (envp);
}

char	**create_list(t_token *tokens)
{
	t_token	*temp;
	char	**lst;
	int		command_flag;
	int		i;
	int		j;

	i = 0;
	temp = tokens;
	lst = malloc((count_content(tokens) + 1) * sizeof(char *));
	if (!lst)
		return (NULL);
	command_flag = is_there_command(temp);
	j = 0;
	if (command_flag)
	{
		while(temp->content[i])
		{
			if (temp->type[i] == COMMAND)
			{
				lst[j] = ft_strdup(temp->content[i]);
				free(temp->content[i]);
				temp->content[i] = ft_strdup("ta3res");
				j++;
				break ;
			}
			i++;
		}
	}
	else if (!command_flag)
	{
		while(temp->content[i])
		{
			if (temp->type[i] == TEXT)
			{
				lst[j] = ft_strdup(temp->content[i]);
				free(temp->content[i]);
				temp->content[i] = ft_strdup("ta3res");
				command_flag = 1;
				j++;
				break ;
			}
			i++;
		}
	}
	i = 0;
	while (temp->content[i] && i < count_content(tokens))
	{
		if (!ft_strcmp(temp->content[i], "ta3res"))
			i++;
		else
		{
			lst[j] = ft_strdup(temp->content[i]);
			i++;
			j++;
		}
	}
	lst[j] = NULL;
	// i = 0;
	// while (lst[i])
	// {
	// 	printf("%s\n", lst[i]);
	// 	i++;
	// }
	// exit(0);
	return (lst);
}

void	execute_command(t_token *tokens, t_shell *shell, t_parser *parser)
{
	char	*cmd;
	char	**lst;
	int			i;

	i = 0;
	lst = create_list(tokens);
	if (shell->enviroment)
		ft_free_2d(shell->enviroment);
	int	id = fork();
	if (id == 0)
	{
		int	j = 0;
		get_paths(shell);
		if (!shell->enviroment)
			shell->enviroment = get_env(shell->env);
		while (shell->paths[j] && shell->paths)
		{
			cmd = ft_strjoin(shell->paths[j], lst[0]);
			if (!cmd)
				exit(EXIT_FAILURE);
			if (!access(cmd, X_OK))
				break;
			free(cmd);
			cmd = NULL;
			j++;
		}
		ft_free_2d(shell->paths);
		if (!cmd)
		{
			ft_printf("command not found:%s\n", lst[0]);
			if (lst)
				ft_free_2d(lst);
			free_tokenizer(tokens);
			free (tokens);
			if(shell->enviroment)
				ft_free_2d(shell->enviroment);
			free_env(shell->env);
			free(shell->env);
			free(shell);
			free(parser);
			exit(EXIT_FAILURE);
		}
		execve(cmd, lst, shell->enviroment);
	}
	wait(NULL);
	i = 0;
	if (lst && lst[i])
	{
		while (lst[i])
		{
			free(lst[i]);
			i++;
		}
		free(lst);
	}
	if(shell->enviroment)
	{
		ft_free_2d(shell->enviroment);
		shell->enviroment = NULL;
	}
}

void	execute(t_shell *shell, t_token *tokens, t_parser *parser)
{
	(void)shell;
	if (!tokens->content)
		return ;
	if (how_many_pipes(tokens) > 0)
	{
		printf("there is pipes : %i\n", how_many_pipes(tokens));
	}
	else
	{
		// if(is_there_command(tokens))
		execute_command(tokens, shell, parser);
		// if (shell->paths)
		// {
		// 	ft_free_2d(shell->paths);
		// 	shell->paths = NULL;
		// }
	}
}
