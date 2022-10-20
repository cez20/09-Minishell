/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cemenjiv <cemenjiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 13:43:50 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/10/19 08:17:20 by cemenjiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h" 

// Avancer dans les tokens, sauf si metacracteres, avancer de 2. Si pipe, avancer de 1
// Quels sont les metacharacteres ‘|’, ‘&’, ‘;’, ‘(’, ‘)’, ‘<’, or ‘>’.  
//1- Il faut que je compte le nombre de pipes et le mettre dans info->nb_of_pipes
// 2- Executer les commandes et leurs arguments jusqua temps qu'il rencontre un pipe
// 3- Si (built-in), faire la commande dans le parent, ELSE, fork() et faire execve 
// 3- Envoyer contenu de la commande dans pipe avec dup2
// 4- Repeter les memes actions jusqu'a ce qu'il n'y ait plus de pipes 

void	split_path(t_info *info)
{
	char **env;
	int		i;
	
	env = info->envp;
	i = 0;
	while (env[i])
	{
		if (ft_strnstr(env[i], "PATH=", 5))
		{
			info->path = ft_split(&env[i][5], ':');
			return ;
		}
		i++;
	}
	printf("There is not PATH in env!\n");
}

int	create_pipes(t_info *info)
{
	int fd[info->nb_of_pipe][2];
	int i;
	
	i = 0;
	while (i < info->nb_of_pipe)
	{
		if (pipe(fd[i]) < 0)
			return (1); // Mettre un message d'erreur de la commande pipe 
		//printf("I've opened the pipe\n");
		i++;
	}
	return (0);
}

void	execution(t_info *info)
{
	int i;
	t_token *tmp;

	i = 0;
	tmp = info->list_token;
	create_pipes(info);
	split_path(info);
	
	
}
