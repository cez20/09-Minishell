/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cemenjiv <cemenjiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 13:43:50 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/11/06 21:27:19 by cemenjiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h" 

// void	put_back_default_std(t_command_line cmd_line, t_info	*info)
// {
// 	if (cmd_line.fd_in != 0)
// 	{
// 		dup2(info->initial_stdin, STDIN_FILENO);
// 		close(info->initial_stdin);
// 	}
// 	if (cmd_line.fd_out != 1)
// 	{
// 		dup2(info->initial_stdout, STDOUT_FILENO);
// 		close(info->initial_stdout);
// 	}
// }

void	put_back_default_std(t_command_line cmd_line, t_info	*info)
{
	(void)cmd_line;
	dup2(info->initial_stdin, STDIN_FILENO);
	close(info->initial_stdin);
	dup2(info->initial_stdout, STDOUT_FILENO);
	close(info->initial_stdout);
}

//Fonction qui execute une commande avec execve() dans un CHILD
//lorsqu'il n'y a aucun PIPE()
void	exec_one_command(t_command_line cmd_line, t_info *info)
{
	pid_t	pid;
	
	pid = fork();
	if (pid == -1)
		return ; 
	if (pid == 0)
	{
		if (cmd_line.merge_path_cmd != NULL)
			execve(cmd_line.merge_path_cmd, cmd_line.cmd_and_args, info->envp);
		printf("bash: %s: command not found\n", cmd_line.cmd_and_args[0]);
		//free_memory(*pipex);
		exit (127);
	}
	//put_back_default_std(cmd_line, info);
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
	(void)info;
	if (cmd_line.fd_in != 0)
	{
		printf("The fd associated with cmd_line.fd_in is %d\n", cmd_line.fd_in);
		dup2(cmd_line.fd_in, STDIN_FILENO);
		printf("The fd associated with stdin is %d\n", STDIN_FILENO);
		close(cmd_line.fd_in);
	}
	if (cmd_line.fd_out != 1)
	{
		printf("The fd associated with cmd_line.fd_out is %d\n", cmd_line.fd_out);
		dup2(cmd_line.fd_out, STDOUT_FILENO);
		close(cmd_line.fd_out);
	}
}

void	create_child(t_command_line cmd_line, t_info *info)
{
	int 	fd[2]; // Les fd qui seront associe 
	pid_t	pid;
	
	if (pipe(fd) == -1) // Creer le pipe() 
		return ;
	pid = fork(); // Creer un fork qui cree un child et un parent process 
	if (pid == -1)
		return ;
	if (pid == 0) // Je rentre dans le child process 
	{
		close(fd[0]); // Je ferme le read-end of pipe, car je veux write dans le child 
		dup2(fd[1], STDOUT_FILENO); // Le resultat de la command cat qui s'affiche normalemnt dans STDOUT sera envoye dans pipe 
		close(fd[1]); // En faisant ceci fd[0] et fd[1] sont ferme dans le child  
		if (cmd_line.merge_path_cmd != NULL)
			execve(cmd_line.merge_path_cmd, cmd_line.cmd_and_args, info->envp);
		printf("bash: %s: command not found\n", cmd_line.cmd_and_args[0]);
		exit (EXIT_FAILURE);
	}
	close(fd[1]);
	dup2(fd[0], STDIN_FILENO); //?? 
	close (fd[0]);
	waitpid(pid, NULL, 0);
}

// void	create_child1(t_command_line cmd_line, t_info *info)
// {
// 	int 	fd[2]; // Les fd qui seront associe 
// 	pid_t	pid;
	
// 	if (pipe(fd) == -1) // Creer le pipe() 
// 		return ;
// 	pid = fork(); // Creer un fork qui cree un child et un parent process 
// 	if (pid == -1)
// 		return ;
// 	if (pid == 0) // Je rentre dans le child process 
// 	{
// 		close(fd[0]); // Je ferme le read-end of pipe, car je veux write dans le child 
// 		dup2(fd[1], STDOUT_FILENO); // Le resultat de la command cat qui s'affiche normalemnt dans STDOUT sera envoye dans pipe 
// 		close(fd[1]); // En faisant ceci fd[0] et fd[1] sont ferme dans le child  
// 		if (cmd_line.merge_path_cmd != NULL)
// 			execve(cmd_line.merge_path_cmd, cmd_line.cmd_and_args, info->envp);
// 		printf("bash: %s: command not found\n", cmd_line.cmd_and_args[0]);
// 		exit (EXIT_FAILURE);
// 	}
// 	close(fd[1]); 
// 	dup2(fd[0], STDIN_FILENO); // Je dois remettre le stdin a son dossier original  = 0; 
// 	close (fd[0]);
// 	waitpid(pid, NULL, 0);
// }

//1- Dans la derniere ligne de commande, il est important de revenir mettre le STDIN_FILENO qui est l,entree du pipe pour
// le STDIN original
void	last_cmd_line(t_command_line cmd_line, t_info *info)
{
	pid_t	pid;
	
	pid = fork();
	if (pid == -1)
		return ;
	if (pid == 0) // Quand je execve , ca ne ferme pas la fonction mais plutot quitte le CHILD> 
	{
		if (cmd_line.merge_path_cmd != NULL)
			execve(cmd_line.merge_path_cmd, cmd_line.cmd_and_args, info->envp);
		printf("bash: %s: command not found\n", cmd_line.cmd_and_args[0]);
		exit (EXIT_FAILURE);
	}
	dup2(info->initial_stdin, STDIN_FILENO); // Dans la 
	waitpid(pid, NULL, 0);
}

// 1- Pourquoi lorsque je change le STDIN pour le fd[0], je ne suis pas oblige de devoir remettre le STDIN original?? 
// 	  ou bien je dois mettre ls STDIN original dans la derniere serie de commande seulement a la TOUTE FIN? 
// 2-- Lorsque j'arrive a la derniere serie de commande, quand il y a eu au moins 1 pipe, est-ce que j'execute dans parent ou child? 
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
		put_back_default_std(cmd_line, info);
	}
	else 
	{
		info->initial_stdin = dup(STDIN_FILENO);
		info->initial_stdout = dup(STDOUT_FILENO);
		while (i < info->nb_of_pipe)
		{
			cmd_line = line[i];
			dup_redirection(cmd_line, info);
			create_child(cmd_line, info);
			i++;
		}
		cmd_line = line[i];
		last_cmd_line(cmd_line, info); // Je pourrais reutiliser mon exec_one_command ou exec_one_builtin;
		put_back_default_std(cmd_line, info);  
	}
}