/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Handler.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalananz <oalananz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 14:23:53 by oalananz          #+#    #+#             */
/*   Updated: 2025/05/05 18:57:41 by oalananz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void exit_execution(t_shell *shell, t_token *tokens, t_parser *parser)
{
    if (tokens)
		free_tokenizer(tokens);
	if (shell->enviroment)
		ft_free_2d(shell->enviroment);
	if (shell->env)
		free_env(shell->env);
	if (shell->cmd_list)
		ft_free_2d(shell->cmd_list);
	free(shell);
	free(parser);
	exit(EXIT_FAILURE);
}

char **list(t_token *tokens)
{
    char	**list;
	t_token	*temp;
	int		i;
    temp = tokens;
    i = 0;
    int count = 0;
    while (temp->content[i])
    {
        if(temp->type[i] == REDIRECTIN || temp->type[i] == HEREDOC || temp->type[i] == REDIRECTOUT || temp->type[i] == APPEND )
            i+=2;
        i++;
        count++;
    }
	list = malloc((count + 1) * sizeof(char *));
	if (!list)
		return (NULL);
    i = 0;
    int index = 0;
	while (temp->content[i])
	{
        
        if(!temp->content[i])
            return NULL;
		list[index] = ft_strdup(temp->content[i]);
		if (!list[index])
		{
            while (--index >= 0) 
                free(list[i]);
			printf("hello\n");
		}
		i++;
        index++;
	}
	list[index] = NULL;
	return (list);
}

void execute_multiple(t_token *tokens, t_shell *shell, t_parser *parser)
{
    char *cmd;
    int j = 0;
    int pipes_count = how_many_pipes(tokens);
    int pids[pipes_count + 1];               // Number of commands = pipes_count + 1
    int pipes[pipes_count][2];               // Number of pipes = pipes_count
    int i = 0;
    int heredoc_flag = 0;

    // Create pipes
    while (i < pipes_count) 
    {
        if (pipe(pipes[i]) == -1) 
        {
            fprintf(stderr, "Error with creating pipe\n");
            return;
        }
        i++;
    }

    i = 0;
    // Create child processes for each command
    while (i < pipes_count + 1) 
    {
        pids[i] = fork();
        if (pids[i] == -1) 
        {
            fprintf(stderr, "Error with creating process\n");
            return;
        }
        if (pids[i] == 0) 
        {
            // Child process
            heredoc_flag = 0;
            shell->cmd_list = list(tokens);
            get_paths(shell);
            if (!shell->paths) 
            {
                char *temp = ft_strjoin("command not found: ",shell->cmd_list[0]);
				char *string = ft_strjoin(temp,"\n");
				free(temp);
				write(2, string, ft_strlen(string));
				free(string);
                exit_execution(shell, tokens, parser);
            }
            if (!shell->enviroment)
                shell->enviroment = get_env(shell->env);
            
            if (shell->cmd_list[0] && (shell->cmd_list[0][0] == '.' || shell->cmd_list[0][0] == '/')) 
            {
                if (!access(shell->cmd_list[0], X_OK))
                    cmd = ft_strdup(shell->cmd_list[0]);
            } 
            else if (shell->cmd_list[0]) 
            {
                j = 0;
                while (shell->paths && shell->paths[j]) 
                {
                    cmd = ft_strjoin(shell->paths[j], shell->cmd_list[0]);
                    if (!cmd)
                        exit(EXIT_FAILURE);
                    if (!access(cmd, X_OK)) 
                        break;
                    free(cmd);
                    cmd = NULL;
                    j++;
                }
                if (shell->paths)
                    ft_free_2d(shell->paths);
                if (!cmd) {
                    char *temp = ft_strjoin("command not found: ",shell->cmd_list[0]);
				    char *string = ft_strjoin(temp,"\n");
				    free(temp);
				    write(2, string, ft_strlen(string));
				    free(string);
                    exit_execution(shell, tokens, parser);
                    exit(EXIT_FAILURE);
                }
            } 
            // else 
            // {
            //     exit_execution(shell, tokens, parser);
            //     exit(0);
            // }
            
            // Close unused pipe ends in the child process
            j = 0;
            while (j < pipes_count) {
                if (i != j + 1) {
                    close(pipes[j][0]);
                }
                if (i != j) {
                    close(pipes[j][1]);
                }
                j++;
            }
            // Redirect input/output for the current command
            if (i == 0) 
            {
                if(is_there_redirect(tokens))
                {
                    int x = 0;
                    int count_rout = 0;
                    int count_rin = 0;
                    int count_append = 0;
                    while(tokens->content[x])
                    {
                        if (tokens->type[x] == REDIRECTOUT) 
                        {
                            count_rout++;
                            int file_fd = open(tokens->content[x + 1], O_CREAT | O_RDWR | O_TRUNC, 0644);
                            if (file_fd == -1) 
                            {
                                perror("Error opening file for redirection\n");
                                close(file_fd);
                                exit(EXIT_FAILURE);
                            }
                            if(count_rout == 1)
                                dup2(pipes[i- 1][0],STDIN_FILENO);
                            dup2(file_fd, STDOUT_FILENO);
                            close(file_fd);
                        }
                        else if(tokens->type[x] == APPEND)
                        {
                            count_append++;
                            int file_fd = open(tokens->content[x + 1], O_CREAT | O_RDWR | O_APPEND, 0644);
                            if (file_fd == -1) 
                            {
                                perror("Error");
                                close(file_fd);
                                exit(EXIT_FAILURE);
                            }
                            if(count_append == 1)
                                dup2(pipes[i - 1][0],STDIN_FILENO);
                            dup2(file_fd, STDOUT_FILENO);
                            close(file_fd);
                        }
                        else if(tokens->type[x] == REDIRECTIN)
                        {
                            count_rin++;
                            int file_fd = open(tokens->content[x + 1], O_RDONLY);
                            if (file_fd == -1) 
                            {
                                perror("Error opening file for redirection\n");
                                close(file_fd);
                                exit(EXIT_FAILURE);
                            }
                            if(count_rin == 1)
                                dup2(pipes[i][1], STDOUT_FILENO);
                            dup2(file_fd, STDIN_FILENO);
                            close(file_fd);
                        }
                        else if(tokens->type[x] == HEREDOC)
                        {
                            char *text = NULL;
                            char *delimiter = tokens->content[x + 1];
                            int fd = open(".t3ree9", O_APPEND | O_CREAT | O_RDWR , 0644);
                            if (delimiter[0] == '\'' || delimiter[0] == '\"')
                            	delimiter = remove_qoutes(delimiter,shell);
                            while (1)
			                {
			                	text = readline(">");
                                if(shell->expand_flag)
			                	    text = expand_heredoc(text, shell);
			                	if (!ft_strcmp(text, delimiter))
			                	{
			                		free (text);
			                		text = NULL;
			                		free(delimiter);
			                		delimiter = NULL;
			                		break ;
			                	}
			                	write(fd, text, ft_strlen(text));
			                	write(fd, "\n", 1);
			                	free(text);
			                }
                            unlink(".t3ree9");
                            dup2(fd,STDIN_FILENO);
                            dup2(pipes[i][1],STDOUT_FILENO);
                            close(fd);
                            free(delimiter);
                        }
                        x++;
                    }
                }
                else
                {
                    dup2(pipes[i][1], STDOUT_FILENO);
                }
            }
            else 
            {
                if(is_there_redirect(tokens))
                {
                    int x = 0;
                    int count_rout = 0;
                    int count_rin = 0;
                    int count_append = 0;
                    while(tokens->content[x])
                    {
                        if (tokens->type[x] == REDIRECTOUT) 
                        {
                            count_rout++;
                            int file_fd = open(tokens->content[x + 1], O_CREAT | O_RDWR | O_TRUNC, 0644);
                            if (file_fd == -1) 
                            {
                                perror("Error opening file for redirection\n");
                                close(file_fd);
                                exit(EXIT_FAILURE);
                            }
                            if(count_rout == 1)
                                dup2(pipes[i - 1][0],STDIN_FILENO);
                            dup2(file_fd, STDOUT_FILENO);
                            close(file_fd);
                        }
                        else if(tokens->type[x] == APPEND)
                        {
                            count_append++;
                            int file_fd = open(tokens->content[x + 1], O_CREAT | O_RDWR | O_APPEND, 0644);
                            if (file_fd == -1) 
                            {
                                perror("Error");
                                close(file_fd);
                                exit(EXIT_FAILURE);
                            }
                            if(count_append == 1)
                                dup2(pipes[i - 1][0],STDIN_FILENO);
                            dup2(file_fd, STDOUT_FILENO);
                            close(file_fd);
                        }
                        else if(tokens->type[x] == REDIRECTIN)
                        {
                            count_rin++;
                            int file_fd = open(tokens->content[x + 1], O_RDONLY);
                            if (file_fd == -1) 
                            {
                                perror("Error opening file for redirection\n");
                                close(file_fd);
                                exit(EXIT_FAILURE);
                            }
                            if(count_rin == 1)
                                dup2(pipes[i][1], STDOUT_FILENO);
                            dup2(file_fd, STDIN_FILENO);
                            close(file_fd);
                        }
                        else if(tokens->type[x] == HEREDOC)
                        {
                            char *text = NULL;
                            char *delimiter = tokens->content[x + 1];
                            int fd = open(".t3ree9", O_APPEND | O_CREAT | O_RDWR , 0644);
                            if (delimiter[0] == '\'' || delimiter[0] == '\"')
                            	delimiter = remove_qoutes(delimiter,shell);
                            while (1)
			                {
			                	text = readline(">");
                                if(shell->expand_flag)
			                	    text = expand_heredoc(text, shell);
			                	if (!ft_strcmp(text, delimiter))
			                	{
			                		free (text);
			                		text = NULL;
			                		free(delimiter);
			                		delimiter = NULL;
			                		break ;
			                	}
			                	write(fd, text, ft_strlen(text));
			                	write(fd, "\n", 1);
			                	free(text);
			                }
                            unlink(".t3ree9");
                            dup2(fd,STDIN_FILENO);
                            dup2(pipes[i][1],STDOUT_FILENO);
                            close(fd);
                            free(delimiter);
                        }
                        x++;
                    }
                }
                else
                {
                    dup2(pipes[i - 1][0], STDIN_FILENO);
                    if (i != pipes_count)
                        dup2(pipes[i][1], STDOUT_FILENO);
                }
            }
            if(is_there_command(tokens))
                execve(cmd, shell->cmd_list, shell->enviroment);
            exit(1);
        }
        tokens = tokens->next;
        free(shell->cmd_list);
        shell->cmd_list = NULL;
        i++;
    }
    // Close all pipe ends in the parent process
    j = 0;
    while (j < pipes_count) {
        close(pipes[j][0]);
        close(pipes[j][1]);
        j++;
    }

    // Wait for all child processes
    i = 0;
    while (i < pipes_count + 1) 
    {
        wait(NULL);
        i++;
    }
    return;
}
