/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close_fds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cemenjiv <cemenjiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 10:22:12 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/11/29 17:34:12 by cemenjiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	close_unused_fds_heredoc(t_command_line *cmd_line, int current_index)
{
	int	i;

	i = 0;
	while (i < current_index)
	{
		if (cmd_line[i].fd_in != 0)
			close (cmd_line[i].fd_in);
		if (cmd_line[i].fd_out != 1)
			close (cmd_line[i].fd_out);
		i++;
	}
}

void	close_current_fds(t_command_line *cmd_line, t_info *info)
{
	int	i;	

	i = info->index;
	if (cmd_line[i].fd_in != 0)
		close (cmd_line[i].fd_in);
	if (cmd_line[i].fd_out != 1)
		close (cmd_line[i].fd_out);
}

void	close_unused_fds(t_command_line *cmd_line, t_info *info)
{
	int	i;

	i = info->index + 1;
	while (i <= info->nb_of_pipe)
	{
		if (cmd_line[i].fd_in != 0)
			close (cmd_line[i].fd_in);
		if (cmd_line[i].fd_out != 1)
			close (cmd_line[i].fd_out);
		i++;
	}
}

void	close_fds(int *fd)
{
	close(fd[1]);
	close(fd[0]);
}
