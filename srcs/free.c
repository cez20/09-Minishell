/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slavoie <slavoie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 13:53:06 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/11/02 16:05:27 by slavoie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_double_pointers(char **args)
{
	int	i;

	i = 0;
	if (!args)
		return ;
	while (args[i])
		free(args[i++]);
	free (args);
}

void	free_struct_command_line(t_info *info)
{
	int	i;

	i = 0;
	while (i < (info->nb_of_pipe + 1))
	{
		free_double_pointers(info->command_lines[i].cmd_and_args);
		//free(info->command_lines[i].command);
		//free(info->command_lines[i].args);
		free(info->command_lines[i].merge_path_cmd);
		if (info->command_lines[i].fd_in != 0)
			close (info->command_lines[i].fd_in);
		if (info->command_lines[i].fd_out != 1)
			close (info->command_lines[i].fd_out);
		i++;
	}	
}
