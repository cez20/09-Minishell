/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cemenjiv <cemenjiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 13:53:06 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/11/15 23:19:36 by cemenjiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_struct_command_line(t_info *info)
{
	int	i;

	i = 0;
	while (i <= info->nb_of_pipe)
	{
		table_flip(info->command_lines[i].argv);
		free(info->command_lines[i].args);
		free(info->command_lines[i].path);
		if (info->command_lines[i].fd_in != 0)
			close (info->command_lines[i].fd_in);
		if (info->command_lines[i].fd_out != 1)
			close (info->command_lines[i].fd_out);
		free(info->command_lines[i].error_infile);
		i++;
	}	
}
