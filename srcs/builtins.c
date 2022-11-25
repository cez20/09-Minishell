/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slavoie <slavoie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/12 09:45:30 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/11/25 12:11:13 by slavoie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	pwd(t_info *info)
{
	if (getcwd(info->pwd, 4096))
		printf("%s\n", getcwd(info->pwd, 4096));
	else
		printf("%s\n", info->pwd);
}

void	echo(t_info *info)
{
	t_token	*token_list;
	int		n;

	n = 0;
	token_list = info->command_lines[info->index].list_token->next;
	quote_remover(info);
	if (token_list)
	{
		while (is_n(token_list)) // ICI
		{
			token_list = token_list->next;
			n++;
		}
	}
	else
	{
		printf("\n");
		return ;
	}
	echo_routine(token_list);
	if (!n)
		printf("\n");
}

void	cd(t_info *info)
{
	char	*new_path;
	char	*oldpwd;
	char	*line;

	oldpwd = ft_calloc(4096, sizeof(char));
	getcwd(oldpwd, 4096);
	if (info->command_lines[info->index].args)
		new_path = info->command_lines[info->index].args;
	else
		new_path = getenv("HOME");
	if (chdir(new_path) != 0 && ((ft_strncmp(new_path, ".", 1) && \
	ft_strncmp(new_path, "..", 2)) || !ft_strncmp(new_path, "...", 3)))
		printf("cd: %s: No such file or directory\n", new_path);
	getcwd(info->pwd, 4096);
	line = search_line(info->envp, "PWD=");
	line = ft_strjoin("PWD=", info->pwd);
	info->envp = tab_trunc(info->envp, "PWD=", 4);
	info->envp = tab_join(info->envp, line);
	free(line);
	line = ft_strjoin("OLDPWD=", oldpwd);
	info->envp = tab_trunc(info->envp, "OLDPWD=", 7);
	info->envp = tab_join(info->envp, line);
	free(line);
	free(oldpwd);
}

void	export(t_info *info)
{
	int		i;
	char	*str;

	str = NULL;
	i = 0;
	if (info->command_lines[info->index].argv[i + 1])
	{
		while (info->command_lines[info->index].argv[i + 1])
		{
			if (check_arg_export(info->command_lines[info->index] \
			.argv[i + 1], info))
				export_routine(info, str, i);
			i++;
		}
	}
	else
		export_no_args(info, str);
}

void	unset(t_info *info)
{
	int		i;
	char	*str;

	i = 0;
	while (info->command_lines[info->index].argv[i + 1])
	{
		if (check_arg_unset(info->command_lines[info->index].argv[i + 1], info))
		{
			str = ft_strjoin(info->command_lines[info->index].argv[i + 1], "=");
			info->envp = tab_trunc(info->envp, str, ft_strlen(str));
			free(str);
		}
		i++;
	}
}
