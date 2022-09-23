/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slavoie <slavoie@student.42quebec.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/15 14:50:27 by slavoie           #+#    #+#             */
/*   Updated: 2022/09/15 14:50:30 by slavoie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_tab(char **tab)
{
	while (*tab)
	{
		printf("str = %s\n", *tab);
		tab++;
	}
}



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


void	echo(char **tab_token)
{
	int i;

	if (!ft_strncmp(tab_token[1], "-n", 2))
		i = 2;
	else
		i = 1;

	while(tab_token[i])
	{
		printf("%s ", tab_token[i++]);
	}
	if (ft_strncmp(tab_token[1], "-n", 2))
		printf("\n");
	
}

char	*search_line(char **tab, char *search)
{
	while(*tab)
	{
		if (ft_strnstr(*tab, search, ft_strlen(search)))
			return(*tab);
		tab++;
	}
	return (NULL);
}


void	pwd(char **envp)
{
	char *pwd;

	pwd = search_line(envp, "PWD=");
	pwd = ft_strchr(pwd, '=');
	printf("%s\n", ++pwd);
}

void token_manager(char **tab_token, char **envp)
{
	// printf("tab_token[0] = %s\n", tab_token[0]);
	// printf("tab_token[1] = %s\n", tab_token[1]);
	// printf("len = %zu\n", ft_strlen(tab_token[0]));
	if (!ft_strncmp(tab_token[0], "echo", 4))
		echo(tab_token);
	if (!ft_strncmp(tab_token[0], "pwd", 3))
		pwd(envp);

}

char simple_or_double(char *token)
{
	while(*token)
	{
		// printf("char = %d\n", *token);
		if (*token == 34|| *token == 39)
			return (*token);
		token++;
	}
	return (0);
}
int	tab_length(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
		i++;
	return (i);
}

char	**tab_join(char **tab, char *line)
{
	int		len;
	char	**new_tab;
	int		i;

	i = 0;
	if (!tab)
	{
		new_tab = ft_calloc(2, sizeof (char *));
		new_tab[i++] = ft_strdup(line);
		new_tab[i] = NULL;
		return (new_tab);
	}
	len = tab_length(tab);
	new_tab = ft_calloc ((len + 2), sizeof(char *));
	while (i < len)
	{
		new_tab[i] = ft_strdup(tab[i]);
		i++;
	}
	new_tab[i++] = ft_strdup(line);
	new_tab[i] = 0;
	table_flip(tab);
	return (new_tab);
}

void	add_token(t_info *info, char *token)
{
	tab_join(info->token, token);
}


char	*search_another_one(char *str, char c, t_info *info)
{
	char *token;
	int i;

	i = 0;

	token = malloc(sizeof(char) * ft_strlen(str));

	while(*str++)
	{
		if(*str == c)
			return(str);
		
		




	}





}

char	**split_token(char *token)
{
	char	**tab;

	if (simple_or_double(token))
		tab = ft_split(token, simple_or_double(token));
	else
		tab = ft_split(token, ' ');

	return (tab);
}

void	init(t_info *info, char **envp)
{
	info->envp = envp;
	info->token = NULL;
}

int main(int argc, char **argv, char **envp)
{
	char *line;
	char **tab_token;
	t_info	info;

	info = malloc(sizeof(t_info));



	printf("Let's go ça part !\n");
	// print_tab(envp);
	rl_replace_line("", 0);
	
	while(1 && argc && argv)
	{
		line = readline("Minishell$>");
		add_history(line);
		rl_redisplay();
		// printf("token = %s", line);
		tab_token = split_token(line);

		token_manager(tab_token, envp);
		// command_exeggutor(line, envp);
		free(line);
	}
	return (0);
}