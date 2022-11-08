/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cemenjiv <cemenjiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 10:13:20 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/11/08 14:52:07 by cemenjiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// Avec l'ancienne fonction, ca ne fait que verifier que le 1ere commande est un builtin.  
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

// void	is_builtin(t_info *info)
// {
// 	t_token	*list;
// 	int		i;

// 	if (!info->command_lines[i].list_token)
// 		return ;
// 	i = 0;
// 	while ((i <= info->nb_of_pipe))
// 	{
// 		list = info->command_lines[i].list_token;
// 		if (ft_strncmp(list->token, "pwd", 3) == 0)
// 			info->command_lines[i].builtin = 1;
// 		else if (ft_strncmp(list->token, "env", 3) == 0)
// 			info->command_lines[i].builtin = 1;
// 		else if (ft_strncmp(list->token, "cd", 2) == 0)
// 			info->command_lines[i].builtin = 1;
// 		else if (ft_strncmp(list->token, "exit", 4) == 0)
// 			info->command_lines[i].builtin = 1;
// 		else if (ft_strncmp(list->token, "export", 6) == 0)
// 			info->command_lines[i].builtin = 1;
// 		else if (ft_strncmp(list->token, "echo", 4) == 0)
// 			info->command_lines[i].builtin = 1;
// 		else if (ft_strncmp(list->token, "unset", 5) == 0)
// 			info->command_lines[i].builtin = 1;
// 		i++;
// 	}
// }

void	find_path_of_command(t_info *info)
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
		while (info->path[j] && info->command_lines[i].builtin != 1 && (info->command_lines[i].list_token))
		{
			if (access(info->command_lines[i].list_token->token, X_OK) != -1)
			{
				cmd_exe = ft_strdup(info->command_lines[i].list_token->token);
				info->command_lines[i].merge_path_cmd = cmd_exe;
				break;
			}	
			path = ft_strjoin(info->path[j], "/");
			cmd_exe = ft_strjoin(path, info->command_lines[i].list_token->token);
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

void	print_double_pointer(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		printf("The value of str[%d] is %s\n", i, str[i]);
		i++;
	}	
}

void	create_execve_args_list(t_info	*info)
{
	t_token	*list;
	char	**str;
	int		i;
	int		j;

	i = 0;
	str = NULL;
	list = info->command_lines[i].list_token;
	if (!list)
		return ;
	while (i < (info->nb_of_pipe + 1))
	{
		j = 0;
		list = info->command_lines[i].list_token;
		info->command_lines[i].cmd_and_args = malloc((ft_lstsize_token(list) + 1) * sizeof(char *));
		str = info->command_lines[i].cmd_and_args;
		while (list)
		{
			str[j] = malloc((ft_strlen(list->token) + 1) * sizeof(char));
			ft_strlcpy(str[j], list->token, ft_strlen(list->token) + 1);
			list = list->next;
			j++;
		}
		str[j] = 0;
		// print_double_pointer(str);
		i++;
	}
}

void	prepare_data_for_execution(t_info *info)
{
	is_builtin(info);
	find_path_of_command(info);
	create_execve_args_list(info);
}
