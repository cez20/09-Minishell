/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cemenjiv <cemenjiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 13:43:50 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/11/03 15:16:30 by cemenjiv         ###   ########.fr       */
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

//Verifier si c'est toujours le 1er element de la node qui constitue la commande

void	execution(t_info *info, t_command_line *line)
{
	t_command_line	cmd_line;
	int				i;
	int 			pid;

	i = 0;
	cmd_line = line[i];
	if (info->nb_of_pipe == 0)
	{
		pid = fork();
		if (pid == -1)
			printf("There is a mistake");
		if (pid == 0)
		{
			if (cmd_line.fd_in != 0)
			{
				dup2(cmd_line.fd_in, STDIN_FILENO);
				close(cmd_line.fd_in);
			}
			if (cmd_line.fd_out != 1)
			{
				dup2(cmd_line.fd_out, STDOUT_FILENO);
				close(cmd_line.fd_out);
			}
			if (cmd_line.builtin == 1)
				token_manager(info);
			else if (cmd_line.merge_path_cmd != NULL && cmd_line.builtin != 1)
				execve(cmd_line.merge_path_cmd, cmd_line.cmd_and_args, info->envp);
			printf("bash: %s: command not found\n", cmd_line.cmd_and_args[0]);
			//free_memory(*pipex);
			exit (EXIT_FAILURE);
		}
		waitpid(pid, NULL, 0);
	}
}
