/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cemenjiv <cemenjiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 13:43:50 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/10/25 19:01:03 by cemenjiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h" 

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
}
