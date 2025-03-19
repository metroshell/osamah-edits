/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oalananz <oalananz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 00:04:10 by oalananz          #+#    #+#             */
/*   Updated: 2025/03/19 22:13:09 by oalananz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    detect_command(t_parser *parser,t_token *temp,char **paths)
{
    char *command_path = NULL;
    int i = 0;
    while(paths[i])
    {
        command_path = ft_strjoin(paths[i],temp->content[parser->index]);
        if(!command_path)
            exit(EXIT_FAILURE);
        if(!access(command_path , X_OK))
        {
            temp->type[parser->index] = COMMAND;
            parser->commands_counter++;
            parser->texts_flag = 1;
            break;
        }
        i++;
    }
    if (command_path)
        free(command_path);
}

void    detect_arguments(t_parser *parser,t_token *temp)
{
    if(temp->content[parser->index][0] == '-' && ft_isalpha(temp->content[parser->index][1]))
    {
        temp->type[parser->index] = ARGUMENT;
        parser->arguments_counter++;
        parser->texts_flag = 1;
    }
}

void    detect_heredoc(t_parser *parser,t_token *temp)
{
    if(ft_strncmp(temp->content[parser->index], "<<", 2) == 0)
    {
        temp->type[parser->index] = HEREDOC;
        parser->heredocs_counter++;
        parser->filename_flag = 1;
        parser->index++;
    }
    else if(ft_strncmp(temp->content[parser->index], ">>", 2) == 0)
    {
        temp->type[parser->index] = APPEND;
        parser->append_counter++;
        parser->filename_flag = 1;
        parser->index++;
    }     
}

void    detect_redirect(t_parser *parser,t_token *temp)
{
    if(*temp->content[parser->index] == '>' ||
        *temp->content[parser->index]== '<')
    {
        temp->type[parser->index] = REDIRECT;
        parser->redirect_counter++;
        parser->filename_flag = 1;
        parser->texts_flag = 1;
        parser->index++;
    }
}

void    detect_filename(t_parser *parser, t_token *temp)
{
    temp->type[parser->index] = FILENAME;
    parser->filename_counter++;
}

void    detect_texts(t_parser *parser, t_token *temp)
{
    temp->type[parser->index] = TEXT;    
    parser->texts_counter++;
    parser->texts_flag = 0;
}  

void    ft_parser(t_token *head,t_shell *shell)
{
    get_paths(shell);
    t_token *temp = head;
    t_parser *parser;
    parser = ft_calloc(1,sizeof(t_parser));
    if(!parser)
        exit(EXIT_FAILURE);
    while (temp)
    {
        while(temp->content[parser->index])
        {
            parser->filename_flag = 0;
            parser->texts_flag = 0;
            detect_command(parser,temp,shell->paths);
            detect_arguments(parser,temp);
            detect_heredoc(parser,temp);
            detect_redirect(parser,temp);
            if(parser->filename_flag)
                detect_filename(parser,temp);
            else if(!parser->texts_flag)
                detect_texts(parser,temp);
            parser->index++;
        }
        parser->index = 0;
        temp = temp->next;
    }
}
