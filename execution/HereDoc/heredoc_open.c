/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_open.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhatahet <qhatahet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 17:40:04 by oalananz          #+#    #+#             */
/*   Updated: 2025/05/25 15:36:37 by qhatahet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_heredoc(t_token *tokens)
{
	t_token	*temp;
	int		counter;
	int		i;

	temp = tokens;
	counter = 0;
	i = 0;
	while (temp->content[i])
	{
		if (temp->type[i] == HEREDOC)
			counter++;
		i++;
	}
	fprintf(stderr, "%d\n", counter);
	return (counter);
}

void	heredoc_ctrl_d(char *text, char *exit_heredoc, t_shell *shell, t_fds *fd)
{
	char	*tmp;
	char	*t;

	tmp = ft_strjoin("ARSSH: here-document delimited by end-of-file (wanted `",
			exit_heredoc);
	t = ft_strjoin(tmp, "\')\n");
	free(tmp);
	write(2, t, ft_strlen(t));
	free(t);
	free(text);
	free(exit_heredoc);
	free_heredoc(shell, fd);
	exit(0);
}

int	open_heredocs(t_shell *shell, char *exit_heredoc, char *file)
{
	char	*text;
	int		fd;

	text = NULL;
	if (exit_heredoc[0] == '\'' || exit_heredoc[0] == '\"')
		exit_heredoc = remove_qoutes(exit_heredoc, shell);
	fd = open(file, O_CREAT | O_RDWR | O_TRUNC, 0644);
	while (1)
	{
		text = readline("> ");
		if (!text)
			heredoc_ctrl_d(text, exit_heredoc, shell, NULL);
		if (shell->expand_flag)
			text = expand_heredoc(text, shell);
		if (text && exit_heredoc && !ft_strcmp(text, exit_heredoc))
		{
			free(text);
			text = NULL;
			break ;
		}
		write(fd, text, ft_strlen(text));
		write(fd, "\n", 1);
		free(text);
	}
	return (fd);
}

int	create_heredoc_files(t_token *tokens)
{
	t_token	*temp;
	int		i;
	int		count;
	char	*str;
	
	temp = tokens;
	count = 1;
	while (temp)
	{
		i = 0;
		while (temp->content[i])
		{
			if (temp->type[i] == HEREDOC)
			{
				str = ft_itoa(count);
				temp->heredoc_file = ft_strjoin(".temp", str);
				free (str);
				count++;
				break ;
			}
			i++;
		}
		temp = temp->next;
	}
	return (count);
}
