/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Handler.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalananz <oalananz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 14:23:53 by oalananz          #+#    #+#             */
/*   Updated: 2025/05/22 19:25:51 by oalananz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int g_exit_status;

void    ft_free_int2d(int **x)
{
        int i = 0;

    while (x[i])
    {
        if (x[i] != NULL) {
            free(x[i]);
        }
        i++;
    }
    free(x);
    x = NULL;
}

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
    if(shell->exe->cmd)
        free(shell->exe->cmd);
    if(shell->exe->pids)
        free(shell->exe->pids);
    if(shell->exe->pipes)
        ft_free_int2d(shell->exe->pipes);
    if(shell->exe)
        free(shell->exe);    
    if(shell)
	    free(shell);
	if(parser)
        free(parser);
}

void exit_execute(t_shell *shell, t_token *tokens)
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

void list_redirect_2(t_token *temp, char **lst, int *i, int *j)
{
    if(temp->type[*i] == HEREDOC || temp->type[*i] == REDIRECTIN
        || temp->type[*i] == REDIRECTOUT || temp->type[*i] == APPEND
        || temp->type[*i] == ENDOFFILE || temp->type[*i] == FILENAME)
        (*i)++;
	else if (temp->type[*i] == COMMAND || temp->type[*i] == ARGUMENT || temp->type[*i] == TEXT )
	{
		lst[*j] = ft_strdup(temp->content[*i]);
		(*i)++;
        (*j)++;
    }
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
        list_redirect_2(temp, lst, &i, &j);
    if(is_there_command(temp) && is_there_heredoc(temp))
    {
		if(temp->heredoc_file)
        	lst[j++] = ft_strdup(temp->heredoc_file);
    }
	lst[j] = NULL;
	return (lst);
}

void copy_command_line_2(int *i, t_token *temp, char **list, int *j)
{
    if(temp->type[*i] == HEREDOC || temp->type[*i] == REDIRECTIN
        || temp->type[*i] == REDIRECTOUT || temp->type[*i] == APPEND
        || temp->type[*i] == ENDOFFILE || temp->type[*i] == FILENAME)
    (*i)++;
    else if (temp->type[*i] == COMMAND || temp->type[*i] == ARGUMENT
        || temp->type[*i] == TEXT )
	{
        list[*j] = ft_strdup(temp->content[*i]);
        if (!list[*j])
            printf("hello\n");
        (*j)++;
        (*i)++;
    }
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
        copy_command_line_2(&i, temp, list, &j);
    if (is_there_command(temp) && is_there_heredoc(temp))
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

static void heredoc_signal_handler(int sig)
{
    (void)sig;
    rl_redisplay();
    write(2, "\n", 1);
	rl_on_new_line();
    exit(128 + SIGINT);
}

void herdoc_ctrl_d(char *text, char *exit_heredoc)
{
    char *tmp;
    char *t;
    
    tmp = ft_strjoin("ARSSH: warning: here-document delimited by end-of-file ( wanted `", exit_heredoc);
    t = ft_strjoin(tmp, "\')\n");
    free(tmp);
    write(2, t, ft_strlen(t));
    free(t);
    free(text);
    exit(0);
}

int    open_heredocs(t_shell *shell,char *exit_heredoc, char *file)
{
    char *text = NULL;
    if (exit_heredoc[0] == '\'' || exit_heredoc[0] == '\"')
        exit_heredoc = remove_qoutes(exit_heredoc,shell);
    int fd = open(file, O_CREAT | O_RDWR | O_TRUNC, 0644);
    while (1)
    {
        text = readline("> ");
        if (!text)
            herdoc_ctrl_d(text, exit_heredoc);
        if(shell->expand_flag)
            text = expand_heredoc(text, shell);
        if (text && exit_heredoc && !ft_strcmp(text, exit_heredoc))
        {
            free(text);
            text = NULL;
            break;
        }
        write(fd, text, ft_strlen(text));
        write(fd, "\n", 1);
        free(text);
    }
    free(exit_heredoc);
    return (fd);
}

void   heredoc_handle(t_token *tokens , t_shell *shell)
{   
    t_token *temp;
    int i;
    int fd;
    pid_t       pid;
    struct sigaction sa;
    struct sigaction original_sa;
    int         status;

	temp = tokens;
	i = 0;
	fd = 0;
    pid = 0;
    sigaction(SIGINT, NULL, &original_sa);
    sa.sa_handler = SIG_IGN;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);
	while (temp)
	{
		i = 0;
		while(temp->content[i])
		{
			if(temp->type[i] == HEREDOC)
			{
                pid = fork();
                if (pid == 0)
                {
                    sa.sa_handler = heredoc_signal_handler;
                    sa.sa_flags = 0;
                    sigemptyset(&sa.sa_mask);
                    sigaction(SIGINT, &sa, NULL);
			    	char *exit_heredoc = ft_strdup(temp->content[i+1]);
			    	if(fd)
			    		close(fd);
			    	fd = open_heredocs(shell, exit_heredoc, temp->heredoc_file);
                    exit(0);
                }
                else if (pid > 0)
                {
                    waitpid(pid, &status, 0);
                    sigaction(SIGINT, &original_sa, NULL);
                    if (WIFEXITED(status))
                    {
                        shell->exit_status = WEXITSTATUS(status);
                        if (shell->exit_status == 128 + SIGINT)
                            return;
                    }
                    else if (WIFSIGNALED(status))
                    {
                        shell->exit_status = 128 + WTERMSIG(status);
                        return;
                    }
                }
            }
			i++;
		}
		if(fd)
			close(fd);
		temp = temp->next;
	}
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

void    cmd_not_found(t_shell *shell)
{
    char *temp; 
    char *string;
    
    temp = ft_strjoin("command not found: ",shell->cmd_list[0]);
    string = ft_strjoin(temp,"\n");
    write(2, string, ft_strlen(string));
    free(temp);
    free(string);    
}

void    path_check(t_shell *shell,t_token *tokens)
{
    get_paths(shell);
    if (!shell->paths)
    {
        cmd_not_found(shell);
        exit_execute(shell, tokens);
        exit(127);
    }
    if (!shell->enviroment)
        shell->enviroment = get_env(shell->env);
    if (shell->cmd_list[0] && (shell->cmd_list[0][0] == '.' || shell->cmd_list[0][0] == '/')) 
    {
        if (!access(shell->cmd_list[0], X_OK))
            shell->exe->cmd = ft_strdup(shell->cmd_list[0]);
        shell->exe->flag = 1;
    }
}

void    cmd_check(t_shell *shell)
{
    shell->exe->j = 0;
    while (shell->paths && shell->paths[shell->exe->j]) 
    {
        shell->exe->cmd = ft_strjoin(shell->paths[shell->exe->j], shell->cmd_list[0]);
        if (!shell->exe->cmd)
            exit(EXIT_FAILURE);
        if (!access(shell->exe->cmd, X_OK)) 
            break;
        free(shell->exe->cmd);
        shell->exe->cmd = NULL;
        shell->exe->j++;
    }
    if (shell->paths)
        ft_free_2d(shell->paths);
}

void    normal_execute(t_shell *shell,t_token *tokens)
{
    shell->exe->flag = 0;
    path_check(shell, tokens);
    if (shell->cmd_list[0] && !shell->exe->flag) 
    {
        cmd_check(shell);
        if (!shell->exe->cmd)
	    {
            cmd_not_found(shell);
            shell->exe->j = 0;
	    	while (shell->exe->j < shell->exe->pipes_count)
	    	{
	    		if (shell->exe->index != shell->exe->j + 1)
	    			close(shell->exe->pipes[shell->exe->j][0]);
	    		if (shell->exe->index != shell->exe->j)
	    			close(shell->exe->pipes[shell->exe->j][1]);
	    		shell->exe->j++;
	    	}
            exit_execute(shell, tokens);
            exit(127);
        }
    }
    signal(SIGQUIT,SIG_DFL);
    signal(SIGINT,SIG_DFL);
    execve(shell->exe->cmd, shell->cmd_list, shell->enviroment);
}

void child_process_2(t_token *tokens, t_shell *shell)
{
    shell->cmd_list = list(tokens);
    shell->exe->j = 0;
    while (shell->exe->j < shell->exe->pipes_count)
	{
        if (shell->exe->index != shell->exe->j + 1)
            close(shell->exe->pipes[shell->exe->j][0]);
        if (shell->exe->index != shell->exe->j)
            close(shell->exe->pipes[shell->exe->j][1]);
        shell->exe->j++;
    }
}

void child_process_3(t_token *tokens, t_shell *shell, int flag)
{
    int x = 0;
    if (flag == 1)
    {
        dup2(shell->exe->pipes[shell->exe->index][1],STDOUT_FILENO);
        close(shell->exe->pipes[shell->exe->index][1]);
    }
    shell->exe->count_rout = 0;
    shell->exe->count_rin = 0;
    shell->exe->count_append = 0;
    while(tokens->content[x])
    {
        if (tokens->type[x] == REDIRECTOUT)
            handle_redirectout(shell,tokens,x);
        else if(tokens->type[x] == APPEND)
            handle_append(shell,tokens,x);
        else if(tokens->type[x] == REDIRECTIN)
            handle_redirectin(shell,tokens, x);
        x++;
    }
}

void dup_and_close(t_shell *shell)
{
    dup2(shell->exe->pipes[shell->exe->index - 1][0], STDIN_FILENO);
    close(shell->exe->pipes[shell->exe->index - 1][0]); 
    if (shell->exe->index != shell->exe->pipes_count)
    {
        dup2(shell->exe->pipes[shell->exe->index][1], STDOUT_FILENO);
        close(shell->exe->pipes[shell->exe->index][1]);
    }
}

void child_process_4(t_token *tokens, t_shell *shell)
{
    if (shell->exe->index == 0) 
    {
        if(is_there_redirect(tokens))
            child_process_3(tokens, shell, 1);
        else
        {
            dup2(shell->exe->pipes[shell->exe->index][1], STDOUT_FILENO);
            close(shell->exe->pipes[shell->exe->index][1]);
        }
    }
    else if (shell->exe->index == shell->exe->pipes_count)
    {
        if(is_there_redirect(tokens))
            child_process_3(tokens, shell, 0);
        else
            dup_and_close(shell);
    }
    else
    {
        if(is_there_redirect(tokens))
            child_process_3(tokens, shell, 1);
        else
            dup_and_close(shell);
    }
}

void    child_process(t_shell *shell,t_token *tokens)
{
    shell->exe->pids[shell->exe->index] = fork();
    if (shell->exe->pids[shell->exe->index] == -1) 
    {
        write(2, "Error with creating process\n", 29);
        return;
    }
    if (shell->exe->pids[shell->exe->index] == 0) 
    {
        child_process_2(tokens, shell);
        child_process_4(tokens, shell);
        if(ft_executor(shell,tokens))
            exit(0);
        else
            normal_execute(shell,tokens);
        exit(1);
    }    
}

void exec_init(t_token *tokens, t_shell *shell)
{
    shell->exe->pipes_count = how_many_pipes(tokens);
    shell->exe->j = 0;
    shell->exe->index = 0;
    shell->exe->pipes = malloc(sizeof(int*) * shell->exe->pipes_count);
    if (shell->exe->pipes == NULL)
        return ;
    while (shell->exe->index < shell->exe->pipes_count) 
    {
        shell->exe->pipes[shell->exe->index] = malloc(sizeof(int) * 2);
        if (shell->exe->pipes[shell->exe->index] == NULL)
            return;
        if (pipe(shell->exe->pipes[shell->exe->index]) == -1)
            return;
        shell->exe->index++;
    }
    shell->exe->index = 0;
}

void exec_commands(t_token *tokens, t_shell *shell)
{
    while (tokens)
    {
        child_process(shell,tokens);
        signal(SIGINT,SIG_IGN);
        tokens = tokens->next;
        free(shell->cmd_list);
        shell->cmd_list = NULL;
        shell->exe->index++;
    }
    shell->exe->j = 0;
    while (shell->exe->j < shell->exe->pipes_count)
	{
        close(shell->exe->pipes[shell->exe->j][0]);
        close(shell->exe->pipes[shell->exe->j][1]);
        shell->exe->j++;
    }
    shell->exe->index = -1;
}

void execute_multiple(t_token *tokens, t_shell *shell)
{
    int status;
    pid_t last_pid;

    shell->exe = ft_calloc(1, sizeof(t_execute));
    if(!shell->exe)
        return;
    exec_init(tokens, shell);
    shell->exe->pids = malloc(sizeof(int)*shell->exe->pipes_count + 1);
    if(!shell->exe->pids)
        return;
    create_heredoc_files(tokens);
	heredoc_handle(tokens,shell);
    exec_commands(tokens, shell);
    last_pid = shell->exe->pids[shell->exe->pipes_count];
    while (++shell->exe->index < shell->exe->pipes_count + 1)
    {
    	shell->exe->pids[shell->exe->index] = wait(&status);
    	if (shell->exe->pids[shell->exe->index] == last_pid)
    	{
    		if (WIFEXITED(status))
    			shell->exit_status = WEXITSTATUS(status);
    		else if (WIFSIGNALED(status))
    			shell->exit_status = 128 + WTERMSIG(status);
    	}
    }
}
