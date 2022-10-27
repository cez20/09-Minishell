/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cemenjiv <cemenjiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 13:43:50 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/10/27 14:35:17 by cemenjiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h" 
// int	create_pipes(t_info *info)
// {
// 	int fd[info->nb_of_pipe][2];
// 	int i;
	
// 	i = 0;
// 	while (i < info->nb_of_pipe)
// 	{
// 		if (pipe(fd[i]) < 0)
// 			return (1); // Mettre un message d'erreur de la commande pipe 
// 		//printf("I've opened the pipe\n");
// 		i++;
// 	}
// 	return (0);
// }

void    find_nb_pipe(t_info *info)
{
    t_token    *list;

    list = info->list_token;
    while (list)
    {
        if (ft_strncmp(list->token, "|", 2) == 0)
            info->nb_of_pipe++;
        list = list->next;
    }
}


//Verifier si c'est toujours le 1er element de la node qui constitue la commande
void	fill_chunk(t_command_line *chunk, t_info *info)
{
	chunk->node = info->list_token; // Contains no linked list.
	chunk->builtin = 0;
	chunk->command = info->list_token->token;
	chunk->fd_in = 0;
	chunk->fd_out = 1;
	chunk->error = 0;
}

void	print_init_chunk(t_command_line *chunk)
{	
	printf("The value of chunk->node is %s\n", (char *)chunk->node);
	printf("The value of chunk->builtin is %d\n", chunk->builtin);
	printf("The value of chunk->cmd is %s\n", chunk->command);
	printf("The value of chunk->fd_in is %d\n", chunk->fd_in);
	printf("The value of chunk->fd_out %d\n", chunk->fd_out);
	printf("The value of chunk->error is %d\n", chunk->error);
}

void	init_chunk(t_command_line *chunk)
{
	chunk->node = NULL; // Contains no linked list.
	chunk->builtin = 0;
	chunk->command = NULL;
	chunk->fd_in = 0;
	chunk->fd_out = 1;
	chunk->error = 0;
}

void	execution(t_info *info)
{
	t_command_line	*chunk;
	int				pid; //Inserer dans une struct existante ou nouvelle

	//This part is to assign memory to chunk and fill it out
	chunk = malloc(sizeof(chunk));
	init_chunk(chunk);
	print_init_chunk(chunk);
	fill_chunk(chunk, info);

	// This part will focus on execution of one command
	find_nb_pipe(info);
	if (info->nb_pipe == 0)
	{
		pid = fork();
		if (pid == -1)
			printf("There is a mistake");
		if (pid == 0) //No need to close fds
		{
			if (chunk->fd_in != 0)
			{
				dup2(chunk->fd_in, STDIN_FILENO);
				close(chunk->fd_in);
			}
			if (chunk->fd_out != 1)
			{
				dup2(chunk->fd_out, STDOUT_FILENO);
				close(chunk->fd_out);
			}
			// Generer un path_exe et un double pointeur, env)
			if (pipex->path_exe1 != NULL && chunk->builtin != 1)
				execve(pipex->path_exe1, pipex->cmd1, info->envp);
			//free_memory(*pipex);
			exit (EXIT_FAILURE);
		}
		waitpid(pid, NULL, 0);
	}
}
