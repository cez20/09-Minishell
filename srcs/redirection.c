/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cemenjiv <cemenjiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 09:55:32 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/11/15 22:18:26 by cemenjiv         ###   ########.fr       */
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
}

void	output_redirection(t_command_line *chunk, char *token)
{	
	if (chunk->fd_out != 1)
		close(chunk->fd_out);
	chunk->fd_out = open(token, O_TRUNC | O_CREAT | O_RDWR, 0644);
}

// Si j'ai plusieurs heredoc comment leur donner des noms differents? 
void	heredoc_redirection(t_command_line *cmd_line, char *delimiter)
{
	char	*line;
	int		fd[2];

	if (cmd_line->fd_in != 0)
		close(cmd_line->fd_in);
	if (pipe(fd) == -1)
		return ;
	while (1)
	{
		line = readline(">");
		if ((ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0) && \
		(ft_strlen(delimiter) == ft_strlen(line)))
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

void	redirection(t_info	*info)
{
	t_command_line	*chunk;
	t_token			*list;
	int				i;

	i = 0;
	while (i <= info->nb_of_pipe)
	{
		chunk = &info->command_lines[i];
		list = info->command_lines[i].list_token;
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
		delete_redirection_tokens(info->command_lines[i]. \
		list_token, &info->command_lines[i].list_token);
		i++;
	}
}
