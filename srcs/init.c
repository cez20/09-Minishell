/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cemenjiv <cemenjiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 14:21:34 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/11/03 11:26:15 by cemenjiv         ###   ########.fr       */
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
		cmd_line[i].cmd_and_args = NULL;
		cmd_line[i].command = NULL;
		cmd_line[i].args = NULL;
		cmd_line[i].builtin = 0;
		cmd_line[i].fd_in = 0;
		cmd_line[i].fd_out = 1;
		cmd_line[i].error_infile = NULL;
		cmd_line[i].merge_path_cmd = NULL;
		i++;
	}
}

void	init_info(t_info *info, char **envp)
{
	info->envp = tabcpy(envp);
	// info->envp = envp;
	info->list_token = NULL;
	// info->flag_quote = 0;
	info->last_position = NULL;
	info->nb_of_pipe = 0;
	info->index = 0;
	info->path = split_path(envp);
	info->state = TEXT;
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
	//free_double_pointers(info->path);
	table_flip(info->path);
	info->path = split_path(info->envp);
	info->index = 0;
	// free(info->list_token);
}

//Fonction qui permet de reinitialiser certaines infos de la struct 
// void	reinit(t_info *info)
// {
// 	int	i;

// 	i = 0;
// 	info->nb_of_pipe = 0;
// 	while (info->command_lines[i].list_token)
// 	{
// 		ft_lstclear_token(&info->command_lines->list_token, free);
// 	}
// 	free(info->command_lines);
// 	free_dpointers(info->path);
// 	info->index = 0;
// 	// free(info->list_token);
// }

// void	reinit(t_info *info)
// {
// 	int	i;

// 	i = 0;
// 	info->nb_of_pipe = 0;
// 	while (i < (info->nb_of_pipe + 1))
// 	{
// 		ft_lstclear_token(&info->command_lines[i].list_token, free);
// 		free_dpointers(info->command_lines[i].cmd_and_args);
// 		free(info->command_lines[i].command);
// 		free(info->command_lines[i].args);
// 		free(info->command_lines[i].merge_path_cmd);
// 		if (info->command_lines[i].fd_in != 0)
// 			close(info->command_lines[i].fd_in);
// 		if (info->command_lines[i].fd_out != 1)
// 			close(info->command_lines[i].fd_out);
// 		i++;
// 	}	
// 	free(info->command_lines);
// 	info->index = 0;
// 	// free(info->list_token);
// }