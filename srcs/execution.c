/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cemenjiv <cemenjiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 13:43:50 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/10/31 11:55:00 by cemenjiv         ###   ########.fr       */
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

//Verifier si c'est toujours le 1er element de la node qui constitue la commande
// void	fill_chunk(t_command_line *chunk, t_info *info)
// {
// 	chunk->node = info->list_token; // Contains no linked list.
// 	chunk->builtin = 0;
// 	chunk->command = info->list_token->token;
// 	chunk->fd_in = 0;
// 	chunk->fd_out = 1;
// 	chunk->error = 0;
// }

// void	execution(t_info *info, t_command_line *line)
// {
// 	t_command_line	chunk;
// 	int				i;
// 	int 			pid;

// 	i = 0;
// 	chunk = line[i];
// 	if (info->nb_of_pipe == 0)
// 	{
// 		pid = fork();
// 		if (pid == -1)
// 			printf("There is a mistake");
// 		if (pid == 0) //No need to close fds
// 		{
// 			if (chunk.fd_in != 0)
// 			{
// 				dup2(chunk.fd_in, STDIN_FILENO);
// 				close(chunk.fd_in);
// 			}
// 			if (chunk.fd_out != 1)
// 			{
// 				dup2(chunk.fd_out, STDOUT_FILENO);
// 				close(chunk.fd_out);
// 			}
// 			if (chunk.builtin == 1)
// 			// Generer un path_exe et un double pointeur, env)
// 			if (pipex->path_exe1 != NULL && chunk->builtin != 1)
// 				execve(pipex->path_exe1, pipex->cmd1, info->envp);
// 			//free_memory(*pipex);
// 			exit (EXIT_FAILURE);
// 		}
// 		waitpid(pid, NULL, 0);
// 	}
// }
