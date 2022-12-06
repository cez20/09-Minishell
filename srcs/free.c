/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cemenjiv <cemenjiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 13:53:06 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/12/02 17:12:57 by cemenjiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// Normalement le free pour command et args n'est plus necessaire car
// il ne malloc pas de nouvelles memoire, mais pointe vers de la memoire
// deja existante 
void	free_struct_command_line(t_info *info)
{
	int	i;

	i = 0;
	if (info->command_lines)
	{
		while (i <= info->nb_of_pipe)
		{
			if (info->command_lines[i].list_token)
				ft_lstclear_token(&info->command_lines[i].list_token, free);
			if (info->command_lines[i].path)
				free(info->command_lines[i].path);
			if (info->command_lines[i].argv)
				table_flip(info->command_lines[i].argv);
			free(info->command_lines[i].error_infile);
			free(info->command_lines[i].error_outfile);
			i++;
		}
	}
	free (info->command_lines);
}

void	free_info(t_info *info)
{
	if (info->envp)
		table_flip(info->envp);
	if (info->pwd)
	{
		free(info->pwd);
		info->pwd = NULL;
	}
	if (info->list_token)
		ft_lstclear_token(&info->list_token, free);
	if (info->paths)
		table_flip(info->paths);
	free(info);
}

void	free_tab(char **tab)
{
	int	i;

	i = 0;
	while (i <= 4)
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

void	free_structs_and_exit(t_info *info, int exit_number)
{
	free_struct_command_line(info);
	free_info(info);
	exit(exit_number);
}
