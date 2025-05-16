/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Handler.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qais <qais@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 14:23:53 by oalananz          #+#    #+#             */
/*   Updated: 2025/05/16 11:50:41 by qais             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int g_exit_status;

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
    if(shell)
	    free(shell);
	if(parser)
        free(parser);
}
int counter(t_token *tokens)
{
    t_token *tmp = tokens;
    int i = 0;
    int count = 0;
    while(tmp->content[i])
    {
        if(tmp->type[i] == HEREDOC)
        {
            i++;
            count++;
        }
        else if(tmp->type[i] == REDIRECTIN || tmp->type[i] == REDIRECTOUT || tmp->type[i] == APPEND  || tmp->type[i] == ENDOFFILE || tmp->type[i] == FILENAME )
            i++;
        else if (tmp->type[i] == COMMAND || tmp->type[i] == ARGUMENT || tmp->type[i] == TEXT )
        {
            count++;
            i++;
        }    
    }
    if(is_there_command(tmp) && is_there_heredoc(tmp))
    {
        count++;   
    }
    return (count);
}

char	**list_redirect(t_token *tokens)
{
	t_token	*temp;
	char	**lst;
	int		i;
	int		j;

	i = 0;
	temp = tokens;
	lst = malloc((counter(tokens) + 1) * sizeof(char *));
	if (!lst)
		return (NULL);
	j = 0;
	while (temp->content[i])
	{
		if(temp->type[i] == HEREDOC || temp->type[i] == REDIRECTIN || temp->type[i] == REDIRECTOUT || temp->type[i] == APPEND  || temp->type[i] == ENDOFFILE || temp->type[i] == FILENAME )
            i++;
		else if (temp->type[i] == COMMAND || temp->type[i] == ARGUMENT || temp->type[i] == TEXT )
		{
			lst[j] = ft_strdup(temp->content[i]);
			if (!lst[j])
				printf("hello\n");
                i++;
                j++;
        }
	}
    if(is_there_command(temp) && is_there_heredoc(temp))
    {
		if(temp->heredoc_file)
        	lst[j++] = ft_strdup(temp->heredoc_file);
    }
	lst[j] = NULL;
	return (lst);
}

char **copy_command_line(t_token *tokens)
{
	char	**list;
	t_token	*temp;
	int		i;
    int     j;

	temp = tokens;
	i = 0;
    j = 0;
	list = malloc((counter(tokens) + 1) * sizeof(char *));
	if (!list)
		return (NULL);
	while (temp->content[i])
	{
        if(temp->type[i] == HEREDOC || temp->type[i] == REDIRECTIN || temp->type[i] == REDIRECTOUT || temp->type[i] == APPEND  || temp->type[i] == ENDOFFILE || temp->type[i] == FILENAME )
            i++;
        else if (temp->type[i] == COMMAND || temp->type[i] == ARGUMENT || temp->type[i] == TEXT )
		{
            list[j] = ft_strdup(temp->content[i]);
            if (!list[j])
                printf("hello\n");
            j++;
            i++;
        }
	}
    if(is_there_command(temp) && is_there_heredoc(temp))
    {
		if(temp->heredoc_file)
        	list[j++] = ft_strdup(temp->heredoc_file);
    }
	list[j] = NULL;
	return (list);
}

char **list(t_token *tokens)
{
    char	**list;

    if (redirect_first_arg(tokens))
		list = list_redirect(tokens);
	else
		list = copy_command_line(tokens);
    // for(int i = 0;list[i] ; i++)
    // {   
    //     fprintf(stderr,"%d:%s\n",i,list[i]);
    // }   
	return (list);
}

int     count_heredoc(t_token *tokens)
{
    t_token *temp = tokens;
    int counter = 0;
    int i = 0;
    while(temp->content[i])
    {
        if(temp->type[i] == HEREDOC)
            counter++;
        i++;
    }
    fprintf(stderr,"%d\n",counter);
    return(counter);
}

int    open_heredocs(t_shell *shell,char *exit, char *file)
{
    char *text = NULL;
    if (exit[0] == '\'' || exit[0] == '\"')
        exit = remove_qoutes(exit,shell);
    int fd = open(file, O_CREAT | O_RDWR | O_TRUNC, 0644);
    while (1)
    {
        text = readline(">");
        if(shell->expand_flag)
            text = expand_heredoc(text, shell);
        if (text && exit && !ft_strcmp(text, exit))
        {
            free(text);
            text = NULL;
            break;
        }
        write(fd, text, ft_strlen(text));
        write(fd, "\n", 1);
        free(text);
    }
    free(exit);
    return (fd);
}

void    heredoc_handle(t_token *tokens , t_shell *shell)
{   
    t_token *temp;
    int i;
    int fd;

	temp = tokens;
	i = 0;
	fd = 0;
	while (temp)
	{
		i = 0;
		while(temp->content[i])
		{
			if(temp->type[i] == HEREDOC)
			{
				char *exit = ft_strdup(temp->content[i+1]);
				if(fd)
					close(fd);
				fd = open_heredocs(shell, exit, temp->heredoc_file);
			}
			i++;
		}
		if(fd)
			close(fd);
		temp = temp->next;
	}
}

int is_there_redirectin(t_token *tokens)
{
    t_token *temp = tokens;

    int i = 0;
    while(temp->content[i])
    {
        if(temp->type[i] == REDIRECTIN)
            return(1);
        i++;
    }
    return (0);
}

int is_there_redirectout(t_token *tokens)
{
    t_token *temp = tokens;

    int i = 0;
    while(temp->content[i])
    {
        if(temp->type[i] == REDIRECTOUT)
            return(1);
        i++;
    }
    return (0);
}

int	create_heredoc_files(t_token *tokens)
{
	t_token	*temp;
	int		i;
	int		count;

	temp = tokens;
	i = 0;
	count = 1;
	while (temp)
	{
		i = 0;
		while (temp->content[i])
		{
			if (temp->type[i] == HEREDOC)
			{
				temp->heredoc_file = ft_strjoin(".temp", ft_itoa(count));
				count++;
				break ;
			}
			i++;
		}
		temp = temp->next;
	}
	return (count);
}

void execute_multiple(t_token *tokens, t_shell *shell, t_parser *parser)
{
    char *cmd;
    int j = 0;
    int pipes_count = how_many_pipes(tokens);
    int pids[pipes_count + 1];               // Number of commands = pipes_count + 1
    int pipes[pipes_count][2];               // Number of pipes = pipes_count
    int i = 0;
    
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
    create_heredoc_files(tokens);
	heredoc_handle(tokens,shell);
    while (tokens)//i < pipes_count + 1)
    {
        pids[i] = fork();
        if (pids[i] == -1) 
        {
            write(2, "Error with creating process\n", 29);
            return;
        }
        if (pids[i] == 0) 
        {
            g_exit_status = 0;
            shell->fd_out = 2;
            // Child process
            shell->cmd_list = list(tokens);
            // Close unused pipe ends in the child process
            j = 0;
            while (j < pipes_count)
			{
                if (i != j + 1)
				{
                    close(pipes[j][0]);
                }
                if (i != j)
				{
                    close(pipes[j][1]);
                }
                j++;
            }
            // Redirect input/output for the current command
            if (i == 0) 
            {
                if(is_there_redirect(tokens))
                {
                    shell->fd_out = dup(pipes[i][1]);
                    dup2(pipes[i][1],STDOUT_FILENO);
                    close(pipes[i][1]);
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
                                perror("Error");
                                close(file_fd);
                                exit(EXIT_FAILURE);
                            }
                            shell->fd_out = dup(file_fd);
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
                            shell->fd_out = dup(file_fd);
                            dup2(file_fd, STDOUT_FILENO);
                            close(file_fd);
                        }
                        else if(tokens->type[x] == REDIRECTIN)
                        {
                            count_rin++;
                            int file_fd = open(tokens->content[x + 1], O_RDONLY);
                            if (file_fd == -1) 
                            {
                                perror("Error");
                                close(file_fd);
                                exit(EXIT_FAILURE);
                            }
                            if(count_rin == 1 && !is_there_redirectout(tokens))
                            {
                                shell->fd_out = dup(pipes[i][1]);
                                dup2(pipes[i][1], STDOUT_FILENO);
                                close(pipes[i][1]);   
                            }
                            dup2(file_fd, STDIN_FILENO);
                            close(file_fd);
                        }
                        x++;
                    }
                }
                else
                {
                    shell->fd_out = dup(pipes[i][1]);
                    dup2(pipes[i][1], STDOUT_FILENO);
                    close(pipes[i][1]);
                }
            }
            else if (i == pipes_count)
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
                                perror("Error");
                                close(file_fd);
                                exit(EXIT_FAILURE);
                            }
                            if(count_rout == 1 && !is_there_redirectin(tokens))
                            {
                                dup2(pipes[i - 1][0],STDIN_FILENO);
                                close(pipes[i - 1][0]); 
                            }
                            shell->fd_out = dup(file_fd);
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
                            {
                                dup2(pipes[i - 1][0],STDIN_FILENO);
                                close(pipes[i - 1][0]); 
                            }
                            shell->fd_out = dup(file_fd);
                            dup2(file_fd, STDOUT_FILENO);
                            close(file_fd);
                        }
                        else if(tokens->type[x] == REDIRECTIN)
                        {
                            count_rin++;
                            int file_fd = open(tokens->content[x + 1], O_RDONLY);
                            if (file_fd == -1) 
                            {
                                perror("Error");
                                close(file_fd);
                                exit(EXIT_FAILURE);
                            }
                            dup2(file_fd, STDIN_FILENO);
                            close(file_fd);
							close(pipes[i - 1][0]);
                        }
                        x++;
                    }
                }
                else
                {
                    dup2(pipes[i - 1][0], STDIN_FILENO);
                    close(pipes[i - 1][0]); 
                    if (i != pipes_count)
                    {
                        shell->fd_out = dup(pipes[i][1]);
                        dup2(pipes[i][1], STDOUT_FILENO);
                        close(pipes[i][1]);
                    }
                }
            }
            else
            {
                if(is_there_redirect(tokens))
                {
                    shell->fd_out = dup(pipes[i][1]);
                    dup2(pipes[i][1], STDOUT_FILENO);
                    close(pipes[i][1]);
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
                                perror("Error");
                                close(file_fd);
                                exit(EXIT_FAILURE);
                            }
                            if(count_rout == 1 && !is_there_redirectin(tokens))
                            {
                                dup2(pipes[i - 1][0], STDIN_FILENO);
                                close(pipes[i - 1][0]);
                            }
                            shell->fd_out = dup(file_fd);
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
                            {
                                dup2(pipes[i - 1][0], STDIN_FILENO);
                                close(pipes[i - 1][0]);
                            }
                            shell->fd_out = dup(file_fd);
                            dup2(file_fd, STDOUT_FILENO);
                            close(file_fd);
                        }
                        else if(tokens->type[x] == REDIRECTIN)
                        {
                            count_rin++;
                            int file_fd = open(tokens->content[x + 1], O_RDONLY);
                            if (file_fd == -1) 
                            {
                                perror("Error");
                                close(file_fd);
                                exit(EXIT_FAILURE);
                            }
                            if(count_rin == 1 && !is_there_redirectout(tokens))
                            {
								fprintf(stderr ,"hello qais\n");
                                shell->fd_out = dup(pipes[i][1]);
                                dup2(pipes[i][1], STDOUT_FILENO);
                                close(pipes[i][1]);
								close(pipes[i - 1][0]);
                            }
                            dup2(file_fd, STDIN_FILENO);
                            close(file_fd);
                        }
                        x++;
                    }
                }
                else
                {
                    dup2(pipes[i - 1][0], STDIN_FILENO);
                    close(pipes[i - 1][0]);
                    if (i != pipes_count)
                    {
                        shell->fd_out = dup(pipes[i][1]);
                        dup2(pipes[i][1], STDOUT_FILENO);
                        close(pipes[i][1]);
                    }
                }
            }
            if(is_there_command(tokens))
            {
                if(ft_executor(shell,tokens))
                {
					fprintf(stderr, "hello theree\n");
                    if(shell->fd_out)
		                close(shell->fd_out);
                    exit(0);
                }
                else
                {
                    get_paths(shell);
                    if (!shell->paths)
                    {
                        char *temp = ft_strjoin("command not found: ",shell->cmd_list[0]);
                        char *string = ft_strjoin(temp,"\n");
                        free(temp);
                        write(2, string, ft_strlen(string));
                        free(string);
                        exit_execution(shell, tokens, parser);
                        exit(127);
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
                        if (!cmd)
			        	{
                            char *temp = ft_strjoin("command not found: ",shell->cmd_list[0]);
                            char *string = ft_strjoin(temp,"\n");
                            free(temp);
                            write(2, string, ft_strlen(string));
                            free(string);
			        		j = 0;
			        		while (j < pipes_count)
			        		{
			        			if (i != j + 1)
			        			{
			        				close(pipes[j][0]);
			        			}
			        			if (i != j)
			        			{
			        				close(pipes[j][1]);
			        			}
			        			j++;
			        		}
                            exit_execution(shell, tokens, parser);
                            exit(127);
                        }
                    }
                    execve(cmd, shell->cmd_list, shell->enviroment);
                }
            }
            exit(1);
        }
		fprintf(stderr, "hello theree\n");
        tokens = tokens->next;
        free(shell->cmd_list);
        shell->cmd_list = NULL;
        i++;
    }
    // Close all pipe ends in the parent process
    j = 0;
    while (j < pipes_count)
	{
        close(pipes[j][0]);
        close(pipes[j][1]);
        j++;
    }
    // Wait for all child processes
    i = 0;
    int status;
    pid_t last_pid = pids[pipes_count];
    while (i < pipes_count + 1)
    {
    	pids[i] = wait(&status);
    
    	if (pids[i] == last_pid)
    	{
    		if (WIFEXITED(status))
    			g_exit_status = WEXITSTATUS(status);
    		else if (WIFSIGNALED(status))
    			g_exit_status = 128 + WTERMSIG(status);
    	}
    	i++;
    }
    return;
}
