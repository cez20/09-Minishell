/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cemenjiv <cemenjiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 09:55:32 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/11/13 19:54:19 by cemenjiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// Qu'arrive-til si le fichier est deja cree? On ne veut pas remplacer le fd? 
void	append_output_redirection(t_command_line *chunk, char *outfile)
{
	if (chunk->fd_out != 1)
		close(chunk->fd_out);
	chunk->fd_out = open(outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (chunk->fd_in == -1)
	{
		printf("bash: %s: %s\n", outfile, strerror(errno));
	}
	//printf("The fd associated with test2.txt est: %d\n", chunk->fd_out);
}

void	output_redirection(t_command_line *chunk, char *token)
{	
	if (chunk->fd_out != 1)
		close(chunk->fd_out);
	chunk->fd_out = open(token, O_TRUNC | O_CREAT | O_RDWR, 0644);
	//printf("The fd associated with test2.txt est: %d\n", chunk->fd_out);
}

// Si j'ai plusieurs heredoc comment leur donner des noms differents? 
void	heredoc_redirection(t_command_line *cmd_line, char *delimiter)
{
	char	*line;
	int	  	fd[2];
	
	if (cmd_line->fd_in != 0)
		close(cmd_line->fd_in);
	if (pipe(fd) == -1)
		return ;
	while (1)
	{
		line = readline(">");
		if ((ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0) && (ft_strlen(delimiter) == ft_strlen(line)))
		{
			free(line);
			break ;
		}
		write(fd[1], line, ft_strlen(line));
		write(fd[1], "\n", 1);
		free(line);
	}
	close (fd[1]);
	cmd_line->fd_in = fd[0]; 
}

void	input_redirection(t_command_line *cmd_line, t_token *list_token)
{
	cmd_line->fd_in = open(list_token->next->token, O_RDWR);
	if (cmd_line->fd_in == -1 && !(cmd_line->error_infile))
	{
		cmd_line->error_infile = ft_strdup(list_token->next->token);
		cmd_line->fd_in = 0;
	}
}

int	is_redirection(t_token *list)
{
	if ((ft_strncmp(list->token, "<", 2) == 0) || (ft_strncmp(list->token, "<<", 2) == 0) ||
		(ft_strncmp(list->token, ">", 2) == 0) || (ft_strncmp(list->token, ">>", 2) == 0))
		return (1);
	else
		return (0);
}

void	delete_tokens(t_token **list)
{
	t_token	*temp;
	t_token	*prev;
	int		i;
	int		nodes_to_erase;

	i = 0;
	temp = NULL;
	prev = NULL;
	nodes_to_erase = 0;
	if ((*list)->next)
		nodes_to_erase = 1;
	while (i <= nodes_to_erase)
	{
		temp = *list;
		prev = (*list)->prev;
		*list = (*list)->next;
		ft_lstdelone_token(temp, free);
		temp = NULL;
		if (*list)
			(*list)->prev = prev;
		if (prev)
			prev->next = *list;
		i++;
	}
}

void	delete_redirection_tokens(t_token *list_token, t_token **list_addr)
{
	t_token	*list;

	list = list_token;
	while (list)
	{
		if (is_redirection(list) == 1 && list->prev)
			delete_tokens(&list);
		else if (is_redirection(list) == 1 && !list->prev)
		{
			delete_tokens(&list);
			*list_addr = list;
		}
		if (list && is_redirection(list) == 0)
			list = list->next;
	}
}

void	redirection(t_info	*info)
{
	t_command_line	*chunk;
	t_token			*list;
	int				i;

	i = 0;
	// lst_print_token(&info->command_lines[i].list_token);
	while (i <= info->nb_of_pipe)
	{
		chunk = &info->command_lines[i];
		list = info->command_lines[i].list_token;
		// lst_print_token(&list);
		while (list)
		{
			if ((ft_strncmp(list->token, "<", 2) == 0) && list->next)
				input_redirection(chunk, list);
			else if ((ft_strncmp(list->token, "<<", 3) == 0) && list->next)
				heredoc_redirection(chunk, list->next->token);
			else if ((ft_strncmp(list->token, ">", 2) == 0) && list->next)
				output_redirection(chunk, list->next->token);
			else if ((ft_strncmp(list->token, ">>", 2) == 0) && list->next)
				append_output_redirection(chunk, list->next->token);
			list = list->next;
		}
		delete_redirection_tokens(info->command_lines[i].list_token, &info->command_lines[i].list_token);
		//delete_redirection_tokens(&info->command_lines[i].list_token);
		// lst_print_token(&info->command_lines[i].list_token);
		i++;
	}
}
