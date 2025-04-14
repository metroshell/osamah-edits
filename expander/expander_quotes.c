/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_quotes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalananz <oalananz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 05:58:19 by oalananz          #+#    #+#             */
/*   Updated: 2025/04/14 16:41:36 by oalananz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	quote_remover(t_token *token, t_expand *expand)
{
	// char	*temp;
	// int		i;

	printf("quotes count = %d\n" ,count_quotes(token, expand));
	// expand->inner = 0;
	// i = 0;
	// temp = malloc(ft_strlen(token->content[expand->outer])
	// 		- count + 1);
	// if (!temp)
	// 	return ;
	// count = 0;
	// while (token->content[expand->outer][expand->inner])
	// {
	// 	if (token->content[expand->outer][expand->inner] == expand->quote)
	// 	{
	// 		count++;
	// 		expand->inner++;
	// 		if(count % 2 == 0)
	// 		{
	// 			while(token->content[expand->outer][expand->inner] && token->content[expand->outer][expand->inner] != '\'' && token->content[expand->outer][expand->inner] != '\"')
	// 				expand->inner++;
	// 			expand->quote = token->content[expand->outer][expand->inner];
	// 			if(expand->quote)
	// 				expand->single_qoute = 1;
	// 		}
	// 	}
	// 	else
	// 		temp[i++] = token->content[expand->outer][expand->inner++];
	// }
	// temp[i] = '\0';
	// free(token->content[expand->outer]);
	// token->content[expand->outer] = temp;
}

// count quotes to be removed 
int	count_quotes(t_token *token, t_expand *expand)
{
	int count;
	count = 0;
	expand->quote = token->content[expand->outer][expand->inner];
	if (expand->quote == '\'')
		expand->single_qoute = 1;
	count++;
	expand->inner++;
	while (token->content[expand->outer][expand->inner])
	{
		if (token->content[expand->outer][expand->inner] == expand->quote)
			count++;
		if(count % 2 == 0)
		{
			while(token->content[expand->outer][expand->inner] && token->content[expand->outer][expand->inner] != '\'' && token->content[expand->outer][expand->inner] != '\"')
				expand->inner++;
			expand->quote = token->content[expand->outer][expand->inner];
		}
		expand->inner++;
	}
	return(count);
}
