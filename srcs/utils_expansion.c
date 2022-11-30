/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_expansion.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slavoie <slavoie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/19 20:18:34 by slavoie           #+#    #+#             */
/*   Updated: 2022/11/30 17:12:20 by slavoie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	chop_chop(char **str, char **tab, char **env, int i)
{
	char	*temp;
	char	*string;

	if (tab[0])
	{
		string = ft_strjoin(tab[0], new_expanded_variable(i, tab[2], env));
		temp = *str;
		*str = ft_strjoin(string, tab[3]);
		free(temp);
		free(string);
	}
	else
	{
		string = ft_strdup(new_expanded_variable(i, tab[2], env));
		temp = *str;
		*str = ft_strjoin(string, tab[3]);
		free(temp);
		free(string);
	}
}

/* Fonction qui trouve l'expansion dans ENV et appelle une autre fonction
pour changer l'expansion par son contenu*/
void	find_expansion(char **str, char **tab, char **env)
{
	char	*string;
	int		i;

	i = 0;
	while (env[i] && ft_strnstr(env[i], tab[2], ft_strlen(tab[2])) == 0)
		i++;
	if (env[i])
	{
		free(*str);
		*str = NULL;
		chop_chop(str, tab, env, i);
	}
	else
	{
		string = *str;
		*str = ft_strjoin(tab[0], tab[3]);
		free(string);
	}
}

void	delimiter_finder(t_info *info, char *delimiter, int fd[])
{
	char	*line;

	close(fd[0]);
	g_fd_in = fd[1];
	while (1)
	{
		line = take_input("> ");
		if ((ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0) && \
		ft_strlen(delimiter) == ft_strlen(line))
		{
			close(fd[1]);
			free(line);
			exit (EXIT_SUCCESS);
		}
		else if (!line)
		{
			close(fd[1]);
			exit(EXIT_SUCCESS);
		}
		locate_expansion(&line, info->envp, info);
		write(fd[1], line, ft_strlen(line));
		write(fd[1], "\n", 1);
		free(line);
	}
}
