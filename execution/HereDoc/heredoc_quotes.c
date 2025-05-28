/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_quotes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qais <qais@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 17:49:11 by oalananz          #+#    #+#             */
/*   Updated: 2025/05/28 06:17:42 by qais             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	quote_counter_expand(char *string, int index, int count, char quote)
{
	while (string[index])
	{
		if (count % 2 == 0)
		{
			while (string[index])
			{
				if (string[index] == '\'' || string[index] == '\"')
				{
					quote = string[index];
					count++;
					index++;
					break ;
				}
				index++;
			}
		}
		if (string[index] == quote)
			count++;
		index++;
	}
	return (count);
}

int	qoutes_counter(char *string)
{
	int		count;
	int		index;
	char	quote;

	count = 0;
	index = 0;
	quote = '\0';
	while (string[index])
	{
		if (string[index] == '\'' || string[index] == '\"')
		{
			quote = string[index];
			count++;
			index++;
			break ;
		}
		index++;
	}
	count += quote_counter_expand(string, index, count, quote);
	return (count);
}

static void	copy_until_first_quote(char *string, char *temp, t_quote_state *st)
{
	while (string[st->index])
	{
		if (string[st->index] == '\'' || string[st->index] == '\"')
		{
			st->quote = string[st->index];
			st->count++;
			st->index++;
			break ;
		}
		else
			temp[(st->i)++] = string[(st->index)++];
	}
}

static void	copy_after_first_quote(char *string, char *temp, t_quote_state *st)
{
	while (string[st->index])
	{
		if (st->count % 2 == 0)
		{
			while (string[st->index])
			{
				if (string[st->index] == '\'' || string[st->index] == '\"')
				{
					st->quote = string[st->index];
					st->count++;
					st->index++;
					break ;
				}
				temp[(st->i)++] = string[(st->index)++];
			}
		}
		if (string[st->index] == st->quote)
		{
			st->count++;
			st->index++;
		}
		else
			temp[(st->i)++] = string[(st->index)++];
	}
}

char	*remove_qoutes(char *string, t_shell *shell)
{
	char			*temp;
	t_quote_state	st;

	temp = malloc(ft_strlen(string) - qoutes_counter(string) + 2);
	if (!temp)
		return (NULL);
	st.index = 0;
	st.i = 0;
	st.count = 0;
	shell->expand_flag = 1;
	st.quote = '\0';
	copy_until_first_quote(string, temp, &st);
	copy_after_first_quote(string, temp, &st);
	temp[st.i] = '\0';
	free(string);
	return (temp);
}
