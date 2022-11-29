/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cemenjiv <cemenjiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 09:55:32 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/11/29 15:08:23 by cemenjiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// Qu'arrive-til si le fichier est deja cree? On ne veut pas remplacer le fd? 
void	append_output_redirection(t_command_line *cmd_line, char *outfile)
{
	if (!cmd_line->error_infile)
	{
		if (cmd_line->fd_out != 1)
			close(cmd_line->fd_out);
		cmd_line->fd_out = open(outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
		cmd_line->fd_out = -1;
		if (cmd_line->fd_out == -1)
		{
			cmd_line->error_outfile = ft_strdup(outfile);
			cmd_line->fd_out = 1;
		}
	}
}

void	output_redirection(t_command_line *cmd_line, char *outfile)
{	
	if (!cmd_line->error_infile)
	{
		if (cmd_line->fd_out != 1)
			close(cmd_line->fd_out);
		cmd_line->fd_out = open(outfile, O_TRUNC | O_CREAT | O_RDWR, 0644);
		if (cmd_line->fd_out == -1)
		{
			cmd_line->error_outfile = ft_strdup(outfile);
			cmd_line->fd_out = 1;
		}
	}
}

void	delimiter_finder(t_info *info, char *delimiter, int fd[])
{
	char	*line;

	close(fd[0]);
	g_fd_in = fd[1];
	while (1)
	{
		line = take_input("> ");
		if ((ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0) && \
		ft_strlen(delimiter) == ft_strlen(line))
		{
			close(fd[1]);
			free(line);
			exit (EXIT_SUCCESS);
		}
		else if (!line)
		{
			close(fd[1]);
			exit(EXIT_SUCCESS);
		}
		locate_expansion(&line, info->envp, info);
		write(fd[1], line, ft_strlen(line));
		write(fd[1], "\n", 1);
		free(line);
	}
}

void	heredoc_redirection(t_command_line *cmd_line, char *delimiter, \
t_info *info, int i)
{
	int		fd[2];
	pid_t	pid;
	int 	status;

	cmd_line->chevron = 1;
	if (pipe(fd) == -1)
		return ;
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, &signal_inside_heredoc);
		close_unused_fds_heredoc(info->command_lines, i);
		delimiter_finder(info, delimiter, fd);
	}
	signal(SIGINT, &signal_outside_heredoc);
	close (fd[1]);
	if (cmd_line->fd_in != 0)
		close (cmd_line->fd_in);
	cmd_line->fd_in = fd[0];
	waitpid(pid, &status, 0);
	info->heredoc = get_exit_code(status);
}

void	search_for_redirection(t_info *info)
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
				input_redirection(chunk, list->next->token);
			else if ((ft_strncmp(list->token, "<<", 3) == 0) && list->next)
				heredoc_redirection(chunk, list->next->token, info, i);
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
