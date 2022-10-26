/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cemenjiv <cemenjiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/15 14:50:27 by slavoie           #+#    #+#             */
/*   Updated: 2022/10/26 18:04:23 by cemenjiv         ###   ########.fr       */
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

/*
	exécute le builtin associer à la première commande
*/
void token_manager(t_info *info)
{

	while (info->index < info->nb_of_pipe + 1)
	{
		if (!ft_strncmp(info->command_lines[info->index].command, "pwd", 3))
			pwd(info);
		if (!ft_strncmp(info->command_lines[info->index].command, "env", 3))
			print_tab(info->envp);
		if (!ft_strncmp(info->command_lines[info->index].command, "cd", 2))
			cd(info);
		if (!ft_strncmp(info->command_lines[info->index].command, "exit", 4))
			exit_terminal();
		if (!ft_strncmp(info->command_lines[info->index].command, "export", 6))
			export(info);
		if (!ft_strncmp(info->command_lines[info->index].command, "echo", 4))
			echo(info);
		info->index++;
	}
}

void	init(t_info *info, char **envp)
{
	info->envp = tabcpy(envp);
	// info->envp = envp;
	info->list_token = NULL;
	// info->flag_quote = 0;
	info->last_position = NULL;
	info->nb_of_pipe = 0;
	info->index = 0;
	info->path = split_path(envp);
}

//Fonction qui permet de reinitialiser certaines infos de la struct 
void	reinit(t_info *info)
{
	int	i;

	i = 0;
	info->nb_of_pipe = 0;
	while (info->command_lines[i].list_token)
	{
		ft_lstclear_token(&info->command_lines->list_token, free);
	}
	free(info->command_lines);
	info->index = 0;
	// free(info->list_token);
}

int main(int argc, char **argv, char **envp)
{
	char *line;
	t_info	*info;
 
	info = ft_calloc(1, sizeof(t_info));
	init(info, envp);

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
		info->nb_of_pipe = how_many(line, '|');
		split_token(line, info);
		var_expansion(info->list_token, envp);
		fill_command_lines(info);
		// token_manager(info);
		redirection(info);
		// execution(info);
		free(line);
		reinit(info);
	}
	// Le free ne peut pas être dans la while, sinon elle créer un segfault à la prochaine itération
	free (info); // Liberer le pointeur declare en debut de fonction main.  
	return (0);
}
