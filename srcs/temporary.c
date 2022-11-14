/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   temporary.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cemenjiv <cemenjiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/13 22:33:48 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/11/13 22:37:18 by cemenjiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



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