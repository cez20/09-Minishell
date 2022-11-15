/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   temporary.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slavoie <slavoie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/13 22:33:48 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/11/14 17:21:38 by slavoie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	create_execve_path_executable(t_info *info)
{
	char	*path;
	char	*cmd_exe;
	int		i;
	int		j;

	if (!info->path)
		return ;
	i = 0;
	while ((i <= info->nb_of_pipe))
	{
		j = 0;
		while (info->path[j] && info->command_lines[i].builtin != 1 \
		&& (info->command_lines[i].list_token))
		{
			if (access(info->command_lines[i].list_token->token, X_OK) != -1)
			{
				cmd_exe = ft_strdup(info->command_lines[i].list_token->token);
				info->command_lines[i].merge_path_cmd = cmd_exe;
				break ;
			}	
			path = ft_strjoin(info->path[j], "/");
			cmd_exe = ft_strjoin(path, info->command_lines[i] \
			.list_token->token);
			free(path);
			if (access(cmd_exe, X_OK) != -1)
			{
				info->command_lines[i].merge_path_cmd = cmd_exe;
				break ;
			}
			free(cmd_exe);
			j++;
		}
		i++;
	}
}

void	create_execve_args_list(t_info	*info, t_command_line *cmd_line)
{
	t_token	*list;
	char	**str;
	int		i;
	int		j;

	i = 0;
	str = NULL;
	if (!cmd_line)
		return ;
	while (i < NB_PROCESS)
	{
		j = 0;
		list = cmd_line[i].list_token;
		if (list)
		{
			cmd_line[i].cmd_and_args = \
			malloc((ft_lstsize_token(list) + 1) * sizeof(char *));
			str = info->command_lines[i].cmd_and_args;
			while (list)
			{
				str[j] = malloc((ft_strlen(list->token) + 1) * sizeof(char));
				ft_strlcpy(str[j], list->token, ft_strlen(list->token) + 1);
				list = list->next;
				j++;
			}
			str[j] = 0;
		}
		else
			str = NULL;
		i++;
	}
}
