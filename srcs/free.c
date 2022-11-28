/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slavoie <slavoie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 13:53:06 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/11/28 16:13:57 by slavoie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_struct_command_line(t_info *info)
{
	int	i;

	i = 0;
	while (i <= info->nb_of_pipe)
	{
		if (info->command_lines[i].argv)
			table_flip(info->command_lines[i].argv);
		if (info->command_lines[i].args)
			free(info->command_lines[i].args);
		if (info->command_lines[i].path)
			free(info->command_lines[i].path);
		free(info->command_lines[i].error_infile);
		if (info->command_lines[i].list_token)
			ft_lstclear_token(&info->command_lines[i].list_token, free);
		i++;
	}
	free (info->command_lines);
}

void	free_info(t_info *info)
{
	if (info->envp)
		table_flip(info->envp);
	if (info->pwd)
		free(info->pwd);
	if (info->list_token)
		ft_lstclear_token(&info->list_token, free);
	if (info->paths)
		table_flip(info->paths);
	free(info);
}

void	free_tab(char **tab)
{
	int i;
	
	i = 0;
	while (i <= 4)
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}
