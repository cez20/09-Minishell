/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cemenjiv <cemenjiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 09:55:32 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/12/02 17:13:14 by cemenjiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	append_output_redirection(t_command_line *cmd_line, char *outfile)
{
	if (!cmd_line->error_infile)
	{
		cmd_line->file_after_chevron = 1;
		if (cmd_line->fd_out != 1)
			close(cmd_line->fd_out);
		cmd_line->fd_out = open(outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
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
		cmd_line->file_after_chevron = 1;
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

void	heredoc_redirection(t_command_line *cmd_line, char *delimiter, \
t_info *info, int i)
{
	int		fd[2];
	pid_t	pid;
	int		status;

	cmd_line->file_after_chevron = 1;
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
	waitpid(pid, &status, 0);
	info->heredoc = get_exit_code(status);
	manage_heredoc_fds(info, cmd_line, fd);
}

int	routine_redirection(int i, t_command_line *chunk, \
t_token *list, t_info *info)
{
	if ((ft_strncmp(list->token, "<<<", 4) == 0) && list->next)
	{
		ft_putstr_fd("bash: syntax error near unexpected token '<'", 2);
		return (0);
	}
	else if ((ft_strncmp(list->token, ">>>", 4) == 0) && list->next)
	{
		ft_putstr_fd("bash: syntax error near unexpected token '>'", 2);
		return (0);
	}
	else if ((ft_strncmp(list->token, "<", 2) == 0) && list->next)
		input_redirection(chunk, list->next->token);
	else if ((ft_strncmp(list->token, "<<", 3) == 0) && list->next)
		heredoc_redirection(chunk, list->next->token, info, i);
	else if ((ft_strncmp(list->token, ">", 2) == 0) && list->next)
		output_redirection(chunk, list->next->token);
	else if ((ft_strncmp(list->token, ">>", 3) == 0) && list->next)
		append_output_redirection(chunk, list->next->token);
	return (1);
}

int	search_for_redirection(t_info *info)
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
			if (!routine_redirection(i, chunk, list, info))
				return (0);
			list = list->next;
		}
		delete_redirection_tokens(info->command_lines[i]. \
		list_token, &info->command_lines[i].list_token);
		i++;
	}
	return (1);
}
