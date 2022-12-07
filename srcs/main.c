/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slavoie <slavoie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/15 14:50:27 by slavoie           #+#    #+#             */
/*   Updated: 2022/12/06 20:04:08 by slavoie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	arg_exit(t_info *info)
{
	if (ft_lstsize_token(info->command_lines[info->index].list_token) > 2)
	{
		printf("exit\n");
		ft_putstr_fd("bash: exit: too many arguments\n", 2);
		info->exit_code = 1;
		return (-42);
	}
	if (info->command_lines[info->index].list_token->next)
	{
		if (!(str_isnum(info->command_lines[info->index] \
		.list_token->next->token)))
		{
			ft_putstr_fd("bash: exit: ", 2);
			ft_putstr_fd(info->command_lines[info->index] \
			.list_token->next->token, 2);
			ft_putstr_fd(": numeric argument required\n", 2);
			return (255);
		}
		else
			return (ft_atoi(info->command_lines[info->index] \
			.list_token->next->token));
	}
	return (info->exit_code);
}

/*
	exécute le builtin associer à la première commande
*/
void	token_manager(t_info *info)
{
	if (!ft_strncmp(info->command_lines[info->index].argv[0], "pwd", 3) \
	&& ft_strlen(info->command_lines[info->index].argv[0]) == 3)
		pwd(info);
	else if (!ft_strncmp(info->command_lines[info->index].argv[0], "env", 3) \
	&& ft_strlen(info->command_lines[info->index].argv[0]) == 3)
		print_tab(info->envp);
	else if (!ft_strncmp(info->command_lines[info->index].argv[0], "cd", 2) \
	&& ft_strlen(info->command_lines[info->index].argv[0]) == 2)
		cd(info);
	else if (!ft_strncmp(info->command_lines[info->index].argv[0], "exit", 4) \
	&& ft_strlen(info->command_lines[info->index].argv[0]) == 4)
		exit_terminal(info, 0, arg_exit(info));
	else if (!ft_strncmp(info->command_lines[info->index].argv[0], "export", 6) \
	&& ft_strlen(info->command_lines[info->index].argv[0]) == 6)
		export(info);
	else if (!ft_strncmp(info->command_lines[info->index].argv[0], "echo", 4) \
	&& ft_strlen(info->command_lines[info->index].argv[0]) == 4)
		echo(info);
	else if (!ft_strncmp(info->command_lines[info->index].argv[0], "unset", 5) \
	&& ft_strlen(info->command_lines[info->index].argv[0]) == 5)
		unset(info);
}

char	*take_input(char *prompt)
{
	char	*line;
	char	*temp;

	line = readline(prompt);
	temp = line;
	if (line)
		line = ft_strtrim(line, " \t\r\v\f\n");
	free(temp);
	return (line);
}

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	t_info	*info;

	info = ft_calloc(1, sizeof(t_info));
	init_info(info, envp);
	printf("Let's go ça part !\n");
	while (1 && argc && argv && envp)
	{
		disable_signals();
		line = take_input("\001"GREEN"\002Minishell\001"RESET"\002$> ");
		if (line && *line)
			add_history(line);
		else if (!line)
		{
			free(line);
			exit_terminal(info, 1, 0);
		}
		routine(info, line);
		reinit(info);
	}
	free (info);
	return (0);
}
