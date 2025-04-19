/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qais <qais@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 02:54:32 by qais              #+#    #+#             */
/*   Updated: 2025/04/01 13:14:44 by qais             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <dirent.h>

void	change_oldpwd(t_shell *shell, char *cwd)
{
	t_env	*oldpwd;
	
	oldpwd = shell->env;
	while (ft_strcmp(oldpwd->variable, "OLDPWD"))
		oldpwd = oldpwd->next;
	if (!ft_strcmp(oldpwd->variable, "OLDPWD"))
	{
		free (oldpwd->content);
		oldpwd->content = ft_strdup(cwd);
	}
}

void	change_pwd(t_shell *shell, char *s)
{
	t_env	*pwd;

	pwd = shell->env;
	if (chdir(s))
		return ;
	while (ft_strcmp(pwd->variable, "PWD"))
		pwd = pwd->next;
	if (!ft_strcmp(pwd->variable, "PWD"))
	{
		free(pwd->content);
		pwd->content = ft_strdup(getcwd(NULL, 0));
	}
}

void	ft_cd(t_shell *shell, t_token *token)
{
	char	cwd[1024];
	int		i;

	i = 0;
	if ((getcwd(cwd, 1024)) == NULL)
		return ;
	if (!ft_strcmp(token->content[i], "cd"))
		i++;
	if (token->content[i])
	{
		if (!access(token->content[i], X_OK))
		{
			change_pwd(shell, token->content[i]);
			change_oldpwd(shell, cwd);
		}
	}
	else if (!token->content[i] || !ft_strcmp(token->content[i], "~"))
		change_pwd(shell, "/home/qais");
	else
		perror("cd");
}
