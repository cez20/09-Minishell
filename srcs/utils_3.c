/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cemenjiv <cemenjiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 10:13:20 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/11/14 11:09:12 by cemenjiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// Avec l'ancienne fonction, ca ne fait que verifier que 
//le 1ere commande est un builtin.  
void	is_builtin(t_info *info)
{
	t_token	*list;
	int		i;

	if (!info->command_lines)
		return ;
	i = 0;
	while ((i <= info->nb_of_pipe) && info->command_lines[i].list_token)
	{
		list = info->command_lines[i].list_token;
		if (ft_strncmp(list->token, "pwd", 3) == 0)
			info->command_lines[i].builtin = 1;
		else if (ft_strncmp(list->token, "env", 3) == 0)
			info->command_lines[i].builtin = 1;
		else if (ft_strncmp(list->token, "cd", 2) == 0)
			info->command_lines[i].builtin = 1;
		else if (ft_strncmp(list->token, "exit", 4) == 0)
			info->command_lines[i].builtin = 1;
		else if (ft_strncmp(list->token, "export", 6) == 0)
			info->command_lines[i].builtin = 1;
		else if (ft_strncmp(list->token, "echo", 4) == 0)
			info->command_lines[i].builtin = 1;
		else if (ft_strncmp(list->token, "unset", 5) == 0)
			info->command_lines[i].builtin = 1;
		i++;
	}
}

void	find_execve_binaries(t_info *info, t_command_line *cmd_line)
{
	char	**path;
	int		i;
	int		j;

	path = info->path;
	if (!path)
		return ;
	i = 0;
	while (i < NB_PROCESS)
	{
		j = 0;
		while (cmd_line[i].list_token && cmd_line[i].builtin != 1 && path[j])
		{
			find_path_of_command(&cmd_line[i], path[j]);
			if (cmd_line[i].merge_path_cmd)
				break ;
			j++;
		}
		i++;
	}
}

void	find_path_of_command(t_command_line *cmd_line, char *path)
{
	char	*temp_exe;
	char	*temp_path;

	if (access((*cmd_line).list_token->token, X_OK) != -1)
		(*cmd_line).merge_path_cmd = ft_strdup((*cmd_line).list_token->token);
	else
	{
		temp_path = ft_strjoin(path, "/");
		temp_exe = ft_strjoin(temp_path, (*cmd_line).list_token->token);
		free(temp_path);
		if (access(temp_exe, X_OK) != -1)
			(*cmd_line).merge_path_cmd = temp_exe;
		else
			free(temp_exe);
	}
}

void	create_execve_args_list(t_info	*info, t_command_line *cmd_line)
{
	t_token	*list;
	int		i;
	int		j;
	int		len;

	i = 0;
	if (!cmd_line)
		return ;
	while (i < NB_PROCESS)
	{
		j = 0;
		list = cmd_line[i].list_token;
		if (list)
		{
			len = ft_lstsize_token(list);
			cmd_line[i].cmd_and_args = malloc((len + 1) * sizeof(char *));
			while (list)
			{
				cmd_line[i].cmd_and_args[j++] = ft_strdup(list->token);
				list = list->next;
			}
			cmd_line[i].cmd_and_args[j] = 0;
		}
		i++;
	}
}

void	print_double_pointer(char **str)
{
	int	i;

	i = 0;
	if (!str)
		return ;
	while (str[i])
	{
		printf("The value of str[%d] is %s\n", i, str[i]);
		i++;
	}	
}

void	prepare_data_for_execution(t_info *info)
{
	is_builtin(info);
	find_execve_binaries(info, info->command_lines);
	create_execve_args_list(info, info->command_lines);
}
