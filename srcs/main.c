/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slavoie <slavoie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/15 14:50:27 by slavoie           #+#    #+#             */
/*   Updated: 2022/11/24 12:06:12 by slavoie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*
	cherche la ligne (char *line) dans le tableau (char **tab) et la renvoie
	si rien n'est trouvé, renvoie NULL 
*/
char	*search_line(char **tab, char *line)
{
	while (*tab)
	{
		if (ft_strnstr(*tab, line, ft_strlen(line)))
			return (*tab);
		tab++;
	}
	return (NULL);
}

/*
	exécute le builtin associer à la première commande
*/
void	token_manager(t_info *info)
{
	if (!ft_strncmp(info->command_lines[info->index].command, "pwd", 3) \
	&& ft_strlen(info->command_lines[info->index].command) == 3)
		pwd(info);
	else if (!ft_strncmp(info->command_lines[info->index].command, "env", 3) \
	&& ft_strlen(info->command_lines[info->index].command) == 3)
		print_tab(info->envp);
	else if (!ft_strncmp(info->command_lines[info->index].command, "cd", 2) \
	&& ft_strlen(info->command_lines[info->index].command) == 2)
		cd(info);
	else if (!ft_strncmp(info->command_lines[info->index].command, "exit", 4) \
	&& ft_strlen(info->command_lines[info->index].command) == 4)
		exit_terminal(info, 0);
	else if (!ft_strncmp(info->command_lines[info->index].command, "export", 6) \
	&& ft_strlen(info->command_lines[info->index].command) == 6)
		export(info);
	else if (!ft_strncmp(info->command_lines[info->index].command, "echo", 4) \
	&& ft_strlen(info->command_lines[info->index].command) == 4)
		echo(info);
	else if (!ft_strncmp(info->command_lines[info->index].command, "unset", 5) \
	&& ft_strlen(info->command_lines[info->index].command) == 5)
		unset(info);
}

char	*take_input(void)
{
	char	*line;
	char	*temp;

	disable_signals();
	line = readline("\033[0;32mMinishell$> \033[0m");
	temp = line;
	if (line)
		line = ft_strjoin(line, "\0");
	free(temp);
	return (line);
}

void	routine(t_info *info, char *line)
{
	if (close_quote_checker(info, line))
		;
	else
	{
		printf("Les quotes ne sont pas fermés.\n");
		free(line);
		return ;
	}
	info->nb_of_pipe = how_many(info, line, '|');
	split_token(line, info);
	if (info->command_lines->list_token)
	{
		search_for_redirection(info);
		var_expansion(info->command_lines, info);
		fill_command_lines(info);
		prepare_data_for_execution(info);
		execution(info, info->command_lines);
		free_struct_command_line(info);
	}
	free(line);
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
		line = take_input();
		if (line)
			add_history(line);
		else
		{
			free(line);
			exit_terminal(info, 1);
		}
		routine(info, line);
		reinit(info);
	}
	free (info);
	return (0);
}
