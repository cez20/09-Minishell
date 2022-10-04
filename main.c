/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slavoie <slavoie@student.42quebec.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/15 14:50:27 by slavoie           #+#    #+#             */
/*   Updated: 2022/10/04 15:34:44 by slavoie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	print_tab(char **tab)
// {
// 	while (*tab)
// 	{
// 		printf("str = %s\n", *tab);
// 		tab++;
// 	}
// }

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


// void	echo(char **tab_token)
// {
// 	int i;

// 	if (!ft_strncmp(tab_token[1], "-n", 2))
// 		i = 2;
// 	else
// 		i = 1;

// 	while(tab_token[i])
// 	{
// 		printf("%s ", tab_token[i++]);
// 	}
// 	if (ft_strncmp(tab_token[1], "-n", 2))
// 		printf("\n");
	
// }

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

// void	pwd(char **envp)
// {
// 	char *pwd;

// 	pwd = search_line(envp, "PWD=");
// 	pwd = ft_strchr(pwd, '=');
// 	printf("%s\n", ++pwd);
// }

void token_manager(char **tab_token, char **envp)
{
	// printf("tab_token[0] = %s\n", tab_token[0]);
	// printf("tab_token[1] = %s\n", tab_token[1]);
	// printf("len = %zu\n", ft_strlen(tab_token[0]));
	if (!ft_strncmp(tab_token[0], "echo", 4))
		echo(tab_token);
	if (!ft_strncmp(tab_token[0], "pwd", 3))
		pwd(envp);
	if (!ft_strncmp(tab_token[0], "env", 3))
		print_tab(envp);

}

char simple_or_double(char *token)
{
	if (*token == 34 || *token == 39)
		return (*token);
	return (32);




	// while(*token)
	// {
	// 	// printf("char = %d\n", *token);
	// 	if (*token == 34 || *token == 39)
	// 		return (*token);
	// 	token++;
	// }
	// return (0);
}
// int	tab_length(char **tab)
// {
// 	int	i;

// 	i = 0;
// 	while (tab[i])
// 		i++;
// 	return (i);
// }

// char	**tab_join(char **tab, char *line)
// {
// 	int		len;
// 	char	**new_tab;
// 	int		i;

// 	i = 0;
// 	if (!tab)
// 	{
// 		new_tab = ft_calloc(2, sizeof (char *));
// 		new_tab[i++] = ft_strdup(line);
// 		new_tab[i] = NULL;
// 		return (new_tab);
// 	}
// 	len = tab_length(tab);
// 	new_tab = ft_calloc ((len + 2), sizeof(char *));
// 	while (i < len)
// 	{
// 		new_tab[i] = ft_strdup(tab[i]);
// 		i++;
// 	}
// 	new_tab[i++] = ft_strdup(line);
// 	new_tab[i] = 0;
// 	table_flip(tab);
// 	return (new_tab);
// }

void	add_token(t_info *info, char *token)
{
	tab_join(info->token, token);
}

void skip_space(t_info *info)
{
	while(is_white_space(*info->last_position))
		info->last_position++;
	// printf("position = %c\n", *info->last_position);
}

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
		// printf("char = %c\n", *str);
	}
	len++;
	info->last_position = ++str;
	token = ft_substr(start, 0, len);
	// printf("token = %s\n", token);
	return (token);

	// char *token;
	// char *begin;
	// int	end;
	// int len;

	// end = start;
	// len = 0;
	// // printf("first = %c\n", *str);
	// while (begin)
	// {
	// 	if (str[start] == c)
	// 	{
	// 		// start++;
	// 		end++;
	// 		// printf("start = %c\n", str[start]);
	// 		while(str[end++] != c)
	// 		{
	// 			printf("str[end] = %c\n", str[end]);
	// 			// end++;
	// 			len++;
	// 		}
	// 		// printf("end = %c\n", str[end]);
	// 		// printf("%d\n", end);
	// 		// printf("%d\n", start);
	// 		info->last_position = &str[end];
	// 		token = ft_substr(str, start, len);
	// 		// printf("token = %s\n", token);
	// 		return (token);
	// 	}
	// 	start++;
	// 	end++;
	// }
	// return (NULL);
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
	// int len;

	i = 0;
	// len = 0;
	info->last_position = token;
	info->token = tab_join(info->token, ft_substr(token, 0, 4));

	info->last_position = &token[4];
	// printf("token %d = %s\n", i, info->token[i]);
	// len +=  ft_strlen(info->token[i++]);
	// printf("len token 1 = %i\n", len);
	// info->token = tab_join(info->token, search_another_one(info->last_position, simple_or_double(info->last_position), info));

	while (*info->last_position)
	{
		skip_space(info);
		info->token = tab_join(info->token, search_another_one(info->last_position, simple_or_double(info->last_position), info));
		skip_space(info);
		// info->last_position++;
		// printf("last_position = %s\n", info->last_position);
		// print_tab(info->token);
	}
	// printf("token %d = %s\n", i, info->token[i]);

	// len +=  ft_strlen(info->token[i++]);
	// printf("len token 2 = %i\n", len);

	
	// info->token = tab_join(info->token, search_another_one(info->last_position, 39, info));
	// printf("token %d = %s\n", i, info->token[i]);

	// len +=  ft_strlen(info->token[i++]);
	// printf("len token 3 = %i\n", len);


	print_tab(info->token);
	free(info->token);
	info->token = ft_calloc(sizeof(char **), 1);








	// if (simple_or_double(token))
	// 	tab = ft_split(token, simple_or_double(token));
	// else
	// 	tab = ft_split(token, ' ');

	// return (tab);
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

		token_manager(info->token, envp);
		// command_exeggutor(line, envp);
		free(line);
	}
	return (0);
}