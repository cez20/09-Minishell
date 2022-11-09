/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slavoie <slavoie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/15 14:50:27 by slavoie           #+#    #+#             */
/*   Updated: 2022/11/09 13:13:24 by slavoie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*
	cherche la ligne (char *line) dans le tableau (char **tab) et la renvoie
	si rien n'est trouvé, renvoie NULL 
*/
char	*search_line(char **tab, char *line)
{
	while(*tab)
	{
		if (ft_strnstr(*tab, line, ft_strlen(line)))
			return(*tab);
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
	if (new_tab)
		new_tab[i] = NULL;
	table_flip(tab);
	return (new_tab);
}

/*
	exécute le builtin associer à la première commande
*/
void token_manager(t_info *info)
{
	while (info->index < info->nb_of_pipe + 1)
	{
		if (!ft_strncmp(info->command_lines[info->index].command, "pwd", 3) && ft_strlen(info->command_lines[info->index].command) == 3)
			pwd(info);
		if (!ft_strncmp(info->command_lines[info->index].command, "env", 3) && ft_strlen(info->command_lines[info->index].command) == 3)
			print_tab(info->envp);
		if (!ft_strncmp(info->command_lines[info->index].command, "cd", 2) && ft_strlen(info->command_lines[info->index].command) == 2)
			cd(info);
		if (!ft_strncmp(info->command_lines[info->index].command, "exit", 4) && ft_strlen(info->command_lines[info->index].command) == 4)
			exit_terminal();
		if (!ft_strncmp(info->command_lines[info->index].command, "export", 6) && ft_strlen(info->command_lines[info->index].command) == 6)
			export(info);
		if (!ft_strncmp(info->command_lines[info->index].command, "echo", 4) && ft_strlen(info->command_lines[info->index].command) == 4)
			echo(info);
		if (!ft_strncmp(info->command_lines[info->index].command, "unset", 5) && ft_strlen(info->command_lines[info->index].command) == 5)
			unset(info);
	info->index++;
	}
}

int	close_quote_checker(t_info *info, char *str)
{
	info->state = TEXT;

	while(*str)
	{
		if (*str == D_QUOTE)
		{
			info->state = D_QUOTE;
			str++;
			while(*str != D_QUOTE && *str)
			{
				str++;
				if (*str == D_QUOTE)
					info->state = TEXT;
			}
			str++;
		}
		if (*str == S_QUOTE)
		{
			info->state = S_QUOTE;
			str++;
			while(*str != S_QUOTE && *str)
			{
				str++;
				if (*str == S_QUOTE)
					info->state = TEXT;
			}
			str++;
		}
		str++;
	}
	if (info->state == TEXT)
		return (1);
	else
		return (0);
}

int main(int argc, char **argv, char **envp)
{
	char *line;
	t_info	*info;

	info = ft_calloc(1, sizeof(t_info));
	init_info(info, envp);

	printf("Let's go ça part !\n");
	disable_echo();
	while(1 && argc && argv && envp)
	{
		signal_modified();
		line = readline("Minishell$> ");
		if (line)
			add_history(line); 
		else 
			exit_terminal();
		if (close_quote_checker(info, line));
			// printf("Les quotes sont tous fermé.\n");
		else
		{
			printf("Les quotes ne sont pas fermés.\n");
			continue ;
		}
		info->nb_of_pipe = how_many(info, line, '|');
		// printf("nb_pipe = %d\n", info->nb_of_pipe);
		split_token(line, info);
		if (info->command_lines->list_token)
		{
			var_expansion(info->command_lines, info);
			fill_command_lines(info);
			//token_manager(info);
			redirection(info);
			prepare_data_for_execution(info);
			print_struct(info->command_lines, info);
			execution(info, info->command_lines);
			free(line);
			free_struct_command_line(info);
		}
		reinit(info); 
	}
	free (info); // Liberer le pointeur declare en debut de fonction main.  
	return (0);
}
