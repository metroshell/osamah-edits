/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_dollar.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalananz <oalananz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 06:02:02 by oalananz          #+#    #+#             */
/*   Updated: 2025/04/05 12:36:33 by oalananz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_dollar(t_shell *shell, t_token *token, t_expand *expand, char *temp, int *i)
{
	t_env *current;

	int count = 0;
	expand->inner++;
	if(ft_isdigit(token->content[expand->outer][expand->inner]) && token->content[expand->outer][expand->inner] == 0)
	{
		char *digit = ft_strdup("bash");
		expand->inner++;
		while(digit[count])
			temp[(*i)++] = digit[count++];
		free(digit);
	}
	else if(ft_isdigit(token->content[expand->outer][expand->inner]))
		expand->inner++;
	else
	{
		while(token->content[expand->outer][expand->inner] && ft_isalpha(token->content[expand->outer][expand->inner]) && token->content[expand->outer][expand->inner] != '_')
		{
			expand->inner++;
			count++;
		}
		expand->variable = ft_substr(token->content[expand->outer],expand->inner - count,count);
		printf("variable -> %s\n",expand->variable);
		current = shell->env;
		while (current)
		{
			if (ft_strcmp(current->variable, expand->variable) == 0)
			{
				int index = 0;
				while(current->content[index])
					temp[(*i)++] = current->content[index++];
				return ;
			}
			current = current->next;
		}
	}
}

int		content_len(t_shell *shell,t_token *token, t_expand *expand)
{
	t_env	*current;

	int count = 0;
	expand->inner++;
	if(ft_isdigit(token->content[expand->outer][expand->inner]) && token->content[expand->outer][expand->inner] == 0)
	{
		expand->inner++;
		count += 4;
	}
	else if(ft_isdigit(token->content[expand->outer][expand->inner]))
		expand->inner++;
	else
		while(token->content[expand->outer][expand->inner] && ft_isalpha(token->content[expand->outer][expand->inner]) && token->content[expand->outer][expand->inner] != '_')
		{
			expand->inner++;
			count++;
		}
	expand->variable = ft_substr(token->content[expand->outer],expand->inner - count,count);
	printf("variable = %s\n",expand->variable);
	current = shell->env;
	while (current)
	{
		if (ft_strcmp(current->variable, expand->variable) == 0)
		{
			count = ft_strlen(current->content);
			printf("count = %d\n",count);
			return (count);
		}
		current = current->next;
	}
	return (count - ft_strlen(expand->variable));
}

int		count_length(t_shell *shell,t_token *token, t_expand *expand)
{
	expand->inner = 0;
	int size = 0;
	while(token->content[expand->outer][expand->inner])
	{
		if(token->content[expand->outer][expand->inner] == '\"')
		{
			size++;
			expand->inner++;
			while(token->content[expand->outer][expand->inner] && token->content[expand->outer][expand->inner] != '\"')
			{
				if(token->content[expand->outer][expand->inner] == '$')
					size += content_len(shell,token,expand);
				expand->inner++;
				size++;
			}
		}
		else if(token->content[expand->outer][expand->inner] == '\'')
		{
			size++;
			expand->inner++;
			while(token->content[expand->outer][expand->inner] && token->content[expand->outer][expand->inner] != '\'')
			{
				expand->inner++;
				size++;
			}
		}
		else if(token->content[expand->outer][expand->inner] == '$')
			size += content_len(shell,token,expand);
		else
		{
			expand->inner++;
			size++;
		}
	}
	expand->inner = 0;
	return (size);
}

void	expand_dollar(t_shell *shell,t_token *token, t_expand *expand)
{
	char	*temp;
	int		i;

	temp = malloc(count_length(shell,token,expand) + 1);
	if(!temp)
		exit(130);
	i = 0;
	while(token->content[expand->outer][expand->inner])
	{
		if(token->content[expand->outer][expand->inner] == '\"')
		{
			temp[i++] = token->content[expand->outer][expand->inner++];
			while(token->content[expand->outer][expand->inner] && token->content[expand->outer][expand->inner] != '\"')
			{
				if(token->content[expand->outer][expand->inner] == '$')
					handle_dollar(shell,token,expand,temp,&i);
				else
					temp[i++] = token->content[expand->outer][expand->inner++];
			}
		}
		else if(token->content[expand->outer][expand->inner] == '\'')
		{
			temp[i++] = token->content[expand->outer][expand->inner++];
			while(token->content[expand->outer][expand->inner] && token->content[expand->outer][expand->inner] != '\'')
				temp[i++] = token->content[expand->outer][expand->inner++];
		}
		else if(token->content[expand->outer][expand->inner] == '$')
			handle_dollar(shell,token,expand,temp,&i);
		else
			temp[i++] = token->content[expand->outer][expand->inner++];
	}
	temp[i] = '\0';
	free(token->content[expand->outer]);
	token->content[expand->outer] = temp;
}
