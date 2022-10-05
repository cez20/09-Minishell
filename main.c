/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slavoie <slavoie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/15 14:50:27 by slavoie           #+#    #+#             */
/*   Updated: 2022/10/05 15:48:39 by slavoie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_path(char *command, char **envp)
{
	char	**path_list;
	char	*full_path;
	char	*temp;
	int		i;

	i = 0;
	while (envp[i] != NULL && ft_strnstr(envp[i], "PATH=", 5) == 0)
		i++;
	printf("path = %s\n", envp[i]);
	if (envp[i] == NULL)
		return (NULL);
	path_list = ft_split(envp[i] + 5, ':');
	i = 0;
	while (path_list[i])
	{
		temp = ft_strjoin(path_list[i], "/");
		full_path = ft_strjoin(temp, command);
		free(temp);
		printf("full_path = %s\n", full_path);
		if (access(full_path, F_OK | X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	table_flip(path_list);
	return (0);
}

void	command_exeggutor(char *argv, char **envp)
{
	char	**command;
	char	*full_path;
	int		i;

	i = -1;
	command = ft_split(argv, ' ');
	print_tab(command);
	if (access(command[0], F_OK | X_OK) == 0)
		full_path = command[0];
	else
		full_path = get_path(command[0], envp);
	printf("%s\n", full_path);
	if (!full_path)
	{
		while (command[++i])
			free(command[i]);
		free(command);
		yo_its_wrong("pas de path\n");
	}
	if (execve(full_path, command, envp) == -1)
		yo_its_wrong("echec exec\n");
}

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
	if (!ft_strncmp(info->token[0], "echo", 4))
		echo(info);
	if (!ft_strncmp(info->token[0], "pwd", 3))
		pwd(info);
	if (!ft_strncmp(info->token[0], "env", 3))
		print_tab(info->envp);
}

/*
	Vérifier la valeur du char à l'adresse du char pointeur "token"
	renvoie la valeur du char si c'est '' ou ""
	sinon renvoie la valeur de l'espace soit '32' 
*/
char simple_or_double(char *token)
{
	if (*token == 34 || *token == 39)
		return (*token);
	return (32);
}

/*
	Avance le pointeur jusqu'à la prchaine chose à interpréter
*/
void skip_space(t_info *info)
{
	while(is_white_space(*info->last_position))
		info->last_position++;
}

/*
	Cherche la prochaine occurence de "c" et renvoie le token
*/
char	*search_another_one(char *str, char c, t_info *info)
{
	char	*token;
	char	*start;
	int		len;

	start = str;
	len = 0;

	str++;
	len++;
	while (*str != c)
	{
		str++;
		len++;
		if (*str == '\0')
		{
			info->last_position = str;
			token = ft_substr(start, 0, len);
			return (token);
		}
	}
	len++;
	info->last_position = ++str;
	token = ft_substr(start, 0, len);
	return (token);
}

// char	*get_command(char *token)
// {



// }

// char *get_token(t_info *info)
// {
// 	int	i;

// 	i = 0;
// 	skip_space(info);
// 	if (*info->last_position != 39 && *info->last_position != 34)
		




// }

void	split_token(char *token, t_info *info)
{
	int i;

	i = 0;
	info->last_position = token;

	while (*info->last_position)
	{
		skip_space(info);
		info->token = tab_join(info->token, search_another_one(info->last_position, simple_or_double(info->last_position), info));
		skip_space(info);
	}
}

void	init(t_info *info, char **envp)
{
	info->envp = envp;
	info->token = NULL;
}

int main(int argc, char **argv, char **envp)
{
	char *line;
	t_info	*info;

	info = malloc(sizeof(t_info));
	init(info, envp);

	printf("Let's go ça part !\n");
	// print_tab(envp);
	disable_echo();
	while(1 && argc && argv && envp)
	{
		signal_modified();
		line = readline("Minishell$>");
		if (line)
			add_history(line);
		else 
			exit_terminal();
		rl_redisplay();
		// printf("token = %s\n", line);
		split_token(line, info);

		token_manager(info);
		// command_exeggutor(line, envp);
		free(line);
		free(info->token);
		info->token = ft_calloc(sizeof(char **), 1);
	}
	return (0);
}