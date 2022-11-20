/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slavoie <slavoie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/15 14:50:27 by slavoie           #+#    #+#             */
/*   Updated: 2022/11/19 20:23:00 by slavoie          ###   ########.fr       */
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

char	**tab_trunc(char **tab, char *str, int len)
{
	int		i;
	char	**new_tab;

	i = 0;
	new_tab = NULL;
	if (!tab || !str)
		return (tab);
	while (tab[i])
	{
		if (ft_strncmp(tab[i], str, len) != 0)
			new_tab = tab_join(new_tab, tab[i]);
		i++;
	}
	table_flip(tab);
	return (new_tab);
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

int	close_quote_checker(t_info *info, char *str)
{
	info->state = TEXT;
	while (str && *str)
	{
		if (*str == D_QUOTE)
		{
			info->state = D_QUOTE;
			while (str && *str)
			{
				str++;
				if (*str && *str == D_QUOTE)
				{
					info->state = TEXT;
					break ;
				}
			}
		}
		if (*str == S_QUOTE)
		{
			info->state = S_QUOTE;
			while (str && *str)
			{
				str++;
				if (*str && *str == S_QUOTE)
				{
					info->state = TEXT;
					break ;
				}
			}
		}
		if (*str)
			str++;
	}
	if (info->state == TEXT)
		return (1);
	else
		return (0);
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

void	garbage_collector(t_info *info)
{
	// free_struct_command_line(info);
	free_info(info);
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
		if (close_quote_checker(info, line))
			;
		else
		{
			printf("Les quotes ne sont pas fermés.\n");
			free(line);
			continue ;
		}
		info->nb_of_pipe = how_many(info, line, '|');
		split_token(line, info);
		if (info->command_lines->list_token)
		{
			var_expansion(info->command_lines, info);
			fill_command_lines(info);
			search_for_redirection(info);
			prepare_data_for_execution(info);
			execution(info, info->command_lines);
			free(line);
			free_struct_command_line(info);
		}
		reinit(info);
	}
	free (info);
	return (0);
}

//Fonction qui intercepte exit_code:
// Macro WIFEXITED retourne true si child a quitte correctement
// Macro WEXITSTATUS permet de transformer le status dans le bon int
// Macro WIFSIGNALED permet de capter si child a quitte avec signal
// WTERMSIG transforme status bon int. On ajoute 128 pour avoir le bon exit_code
int	get_exit_code(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (0);
}