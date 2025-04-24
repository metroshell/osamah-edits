/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalananz <oalananz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 05:18:33 by oalananz          #+#    #+#             */
/*   Updated: 2025/04/24 19:17:02 by oalananz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// print env
void	print_env(t_env *env)
{
	t_env	*current;

	current = env;
	while (current)
	{
		if (current->variable && current->content)
			printf("%s=%s\n", current->variable, current->content);
		else if (current->variable)
			printf("%s=\n", current->variable);
		current = current->next;
	}
}

void	env_edit(t_shell *shell)
{
	t_env	*current;

	current = shell->env;
	while (current)
	{
		if (ft_strcmp(current->variable, "SHLVL") == 0)
		{
			int a = ft_atoi(current->content);
			a++;
			if(a > 999)
			{
				printf("arssh: warning: shell level (%d) too high, resetting to 1\n",a);
				a = 1;
			}
			char *temp = ft_itoa(a);
			free(current->content);
			current->content = temp;
			return ;
		}
		current = current->next;
	}
}
