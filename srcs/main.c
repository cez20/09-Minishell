/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slavoie <slavoie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/15 14:50:27 by slavoie           #+#    #+#             */
/*   Updated: 2022/10/31 14:31:45 by slavoie          ###   ########.fr       */
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
		return (0);
	while (tab[i] && ft_strncmp(tab[i], str, len) != 0)
	{
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
		if (!ft_strncmp(info->command_lines[info->index].command, "unset", 5))
			info->envp = tab_trunc(info->envp, info->command_lines[info->index].args, ft_strlen(info->command_lines[info->index].args));

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
	info->state = TEXT;
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

int	close_quote_checker(t_info *info, char *str)
{
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
void	is_builtin(t_info *info)
{
	int i;

	i = 0;
	while ((i <= info->nb_of_pipe) && (info->command_lines[i].list_token))
	{
		if (ft_strncmp(info->command_lines[i].list_token->token, "pwd", 3) == 0)
			info->command_lines[i].builtin = 1;
		else if (ft_strncmp(info->command_lines[i].list_token->token, "env", 3) == 0)
			info->command_lines[i].builtin = 1;
		else if (ft_strncmp(info->command_lines[i].list_token->token, "cd", 2) == 0)
			info->command_lines[i].builtin = 1;
		else if (ft_strncmp(info->command_lines[i].list_token->token, "exit", 4) == 0)
			info->command_lines[i].builtin = 1;
		else if (ft_strncmp(info->command_lines[i].list_token->token, "export", 6) == 0)
			info->command_lines[i].builtin = 1;
		else if (ft_strncmp(info->command_lines[i].list_token->token, "echo", 4) == 0)
			info->command_lines[i].builtin = 1;
		else if (ft_strncmp(info->command_lines[i].list_token->token, "unset", 5) == 0)
			info->command_lines[i].builtin = 1;
		i++;
	}
}

void fill_cmd(t_info *info)
{
	char	*path;
	char	*cmd_exe;
	int		i;
	int		j;
	
	i = 0;
	j = 0;
	while ((i <= info->nb_of_pipe) && (info->command_lines[i].list_token))
	{
		while (info->path[j])
		{
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

void	prepare_data_for_execution(t_info *info)
{
	is_builtin(info);
	fill_cmd(info); // Il faut que je change pour l'adresse car sinon l'info ne suit pas
}

int main(int argc, char **argv, char **envp)
{
	char *line;
	t_info	*info;
	int i;
 
	i = 0; 
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
		if (close_quote_checker(info, line))
			printf("Les quotes sont tous fermé.\n");
		else
			printf("Les quotes ne sont pas fermés.\n");
		info->nb_of_pipe = how_many(info, line, '|');
		// printf("nb_pipe = %d\n", info->nb_of_pipe);
		split_token(line, info);
		var_expansion(info->list_token, envp);
		//fill_command_lines(info);
		//token_manager(info);
		redirection(info);
		//prepare_data_for_execution(info);
		//fill_command_lines(info);
		//execution(info, info->command_lines);
		free(line);
		reinit(info);
	}
	// Le free ne peut pas être dans la while, sinon elle créer un segfault à la prochaine itération
	free (info); // Liberer le pointeur declare en debut de fonction main.  
	return (0);
}
