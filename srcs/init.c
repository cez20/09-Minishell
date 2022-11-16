/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cemenjiv <cemenjiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 14:21:34 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/11/15 23:20:05 by cemenjiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	init_command_lines(t_command_line *cmd_line, t_info *info)
{
	int	i;

	i = 0;
	while (i < (info->nb_of_pipe + 1))
	{
		cmd_line[i].list_token = NULL;
		cmd_line[i].argv = NULL;
		cmd_line[i].command = NULL;
		cmd_line[i].args = NULL;
		cmd_line[i].builtin = 0;
		cmd_line[i].fd_in = 0;
		cmd_line[i].fd_out = 1;
		cmd_line[i].error_infile = NULL;
		cmd_line[i].path = NULL;
		i++;
	}
}

void	init_info(t_info *info, char **envp)
{
	info->pwd = ft_strdup(getcwd(info->pwd, 4096));
	info->envp = tabcpy(envp);
	info->list_token = NULL;
	info->last_position = NULL;
	info->nb_of_pipe = 0;
	info->index = 0;
	info->initial_stdin = 0;
	info->initial_stdout = 1;
	info->paths = split_path(envp);
	info->state = TEXT;
	info->exit_code = 0;
	info->initial_stdin = dup(STDIN_FILENO);
	info->initial_stdout = dup(STDOUT_FILENO);
}

void	reinit(t_info *info)
{
	int	i;

	i = 0;
	info->nb_of_pipe = 0;
	while (i <= info->nb_of_pipe)
	{
		ft_lstclear_token(&info->command_lines[i].list_token, free);
		i++;
	}
	free(info->command_lines);
	table_flip(info->paths);
	info->paths = split_path(info->envp);
	info->index = 0;
	info->initial_stdin = dup(STDIN_FILENO);
	info->initial_stdout = dup(STDOUT_FILENO);
}
