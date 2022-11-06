/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cemenjiv <cemenjiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 13:43:50 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/11/05 17:58:17 by cemenjiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h" 

void	put_back_default_std(t_command_line cmd_line, t_info	*info)
{
	if (cmd_line.fd_in != 0)
	{
		dup2(info->initial_stdin, STDIN_FILENO);
		close(info->initial_stdin);
	}
	if (cmd_line.fd_out != 1)
	{
		dup2(info->initial_stdout, STDOUT_FILENO);
		close(info->initial_stdout);
	}
}

//Fonction qui execute une commande avec execve() dans un CHILD
//lorsqu'il n'y a aucun PIPE()
void	exec_one_command(t_command_line cmd_line, t_info *info)
{
	pid_t	pid;
	
	pid = fork();
	if (pid == -1)
		printf("There is a mistake");
	if (pid == 0)
	{
		if (cmd_line.merge_path_cmd != NULL)
			execve(cmd_line.merge_path_cmd, cmd_line.cmd_and_args, info->envp);
		printf("bash: %s: command not found\n", cmd_line.cmd_and_args[0]);
		//free_memory(*pipex);
		exit (127);
	}
	put_back_default_std(cmd_line, info);
	waitpid(pid, NULL, 0);
}

// Fonction qui execute un builtin dans le PARENT quand il n'y aucun PIPE 
void exec_one_builtin(t_command_line cmd_line)
{
	(void)cmd_line;
	//token_manager();
	//put_back_default_std(cmd_line, info);
}

//Fonction that changes STDIN and STDOUT with dup2()in PARENT before entering 
//CHILD process. I need to keep initial STDIN and STDOUT in memoru for later 
void	dup_redirection(t_command_line cmd_line, t_info *info)
{
	if (cmd_line.fd_in != 0)
	{
		info->initial_stdin = dup(STDIN_FILENO);
		dup2(cmd_line.fd_in, STDIN_FILENO);
		close(cmd_line.fd_in);
	}
	if (cmd_line.fd_out != 1)
	{
		info->initial_stdout = dup(STDOUT_FILENO);
		dup2(cmd_line.fd_out, STDOUT_FILENO);
		close(cmd_line.fd_out);
	}
}

// void	create_pipe(t_info *info, int fd[][2], int i)
// {
// 	if (pipe(fd[i]) == -1)
// 		return ; // Mettre un message d'erreur de la commande pipe 
// 	printf("I've created pipe %d\n", i);
// }

void	create_child(t_command_line cmd_line, t_info *info)
{
	int 	fd[2];
	pid_t	pid;
	
	if (pipe(fd) == -1)
		return ;
	pid = fork();
	if (pid == -1)
		return ;
	if (pid == 0) // Quand je execve , ca ne ferme pas la fonction mais plutot quitte le CHILD> 
	{
		printf("Je suis dans le modulo pair\n");
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]); // Il est mis dans le parent process juste ici bas. 
		if (cmd_line.merge_path_cmd != NULL)
			execve(cmd_line.merge_path_cmd, cmd_line.cmd_and_args, info->envp);
		printf("bash: %s: command not found\n", cmd_line.cmd_and_args[0]);
		exit (EXIT_FAILURE);
	}
	//close(fd[1]);  // Je ferme le write end of pipe 
	dup2(fd[0], STDIN_FILENO);  //  On revient dans le parent. On change le STDIN pour le read-end of pipe avant de fork() de nouveau 
	waitpid(pid, NULL, 0); // Attends pour le pid. 

}

void	last_cmd_line(t_command_line cmd_line, t_info *info)
{
	int 	fd[2];
	pid_t	pid;
	
	pid = fork();
	if (pid == -1)
		return ;
	if (pid == 0) // Quand je execve , ca ne ferme pas la fonction mais plutot quitte le CHILD> 
	{
		close(fd[1]); // Close write end of pipe 
		dup2(fd[0], STDIN_FILENO); // Change le STDIN qui sera utilise pour le 2e process par le read end of pipe.  
		close(fd[0]); // Close the read end of pipe 
		if (cmd_line.merge_path_cmd != NULL)
			execve(cmd_line.merge_path_cmd, cmd_line.cmd_and_args, info->envp);
		printf("bash: %s: command not found\n", cmd_line.cmd_and_args[0]);
		exit (EXIT_FAILURE);
	}
	close(fd[1]);
	waitpid(pid, NULL, 0);
}

void	execution(t_info *info, t_command_line *line)
{
	t_command_line	cmd_line;
	int				i;
	
	i = 0;
	if (info->nb_of_pipe == 0)
	{
		cmd_line = line[i];
		dup_redirection(cmd_line, info);
		if (cmd_line.builtin == 1)
			exec_one_builtin(cmd_line);
		else if (cmd_line.builtin == 0)
			exec_one_command(cmd_line, info);
	}
	else 
	{
		while (i < info->nb_of_pipe)
		{
			cmd_line = line[i];
			create_child(cmd_line, info);
			i++;
		}
		cmd_line = line[i];
		last_cmd_line(cmd_line, info);
	}
}