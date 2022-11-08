/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cemenjiv <cemenjiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 13:43:50 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/11/07 21:49:36 by cemenjiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h" 

// Fonction qui execute un builtin dans le PARENT quand il n'y aucun PIPE 
void exec_one_builtin(t_command_line cmd_line, t_info *info)
{
	(void)cmd_line;
	token_manager(info);
}

void	put_back_default_std(t_info *info)
{
	dup2(info->initial_stdin, STDIN_FILENO);
	close(info->initial_stdin);
	dup2(info->initial_stdout, STDOUT_FILENO);
	close(info->initial_stdout);
}

//If command is not valid. Verify that something needs to be freed or not? 
void exec_one_command(t_command_line cmd_line, t_info *info)
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
		exit (EXIT_FAILURE);
	}
	else
		waitpid(pid, NULL, 0);	
}

//Fonction that changes STDIN and STDOUT with dup2()in PARENT before entering 
//CHILD process. I need to keep initial STDIN and STDOUT in memoru for later 
void	dup_redirection(t_command_line cmd_line)
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
}

//Fonction qui execute une commande avec execve() dans un CHILD
//lorsqu'il n'y a aucun PIPE()
void	one_command_or_builtin(t_command_line cmd_line, t_info *info)
{
	dup_redirection(cmd_line);
	if (cmd_line.builtin == 1)
		token_manager(info); 
	else
		exec_one_command(cmd_line, info);
}

void	create_child(t_command_line cmd_line, t_info *info, pid_t pid)
{
	int 	fd[2]; // Les fd qui seront associe 
	//pid_t	pid;
	
	if (pipe(fd) == -1) // Creer le pipe() 
		return ;
	pid = fork(); // Creer un fork qui cree un child et un parent process 
	if (pid == -1)
		return ;
	if (pid == 0) // Je rentre dans le child process 
	{
		close(fd[0]); // Je ferme le read-end of pipe, car je veux write dans le child 
		dup2(fd[1], STDOUT_FILENO); // Le resultat de la command cat qui s'affiche normalemnt dans STDOUT sera envoye dans pipe 
		close(fd[1]); // Pas besoin de close (fd1, car dup2 s'en 
		if (cmd_line.merge_path_cmd != NULL)
			execve(cmd_line.merge_path_cmd, cmd_line.cmd_and_args, info->envp);
		printf("bash: %s: command not found\n", cmd_line.cmd_and_args[0]);
		exit (EXIT_FAILURE);
	}
	else
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO); //?? 
		close (fd[0]); //Not necessary because dup2 closes it??
	//waitpid(pid, NULL, 0);
	}
}

//1- Dans la derniere ligne de commande, il est important de revenir mettre le STDIN_FILENO qui est l,entree du pipe pour
// le STDIN original
void	last_cmd_line(t_command_line cmd_line, t_info *info, pid_t pid)
{
	//pid_t	pid;
	
	pid = fork();
	if (pid == -1)
		return ;
	if (pid == 0) // Quand je execve , ca ne ferme pas la fonction mais plutot quitte le CHILD> 
	{
		if (cmd_line.builtin == 1)
		{
			token_manager(info);
			exit (EXIT_FAILURE);
		}
		if (cmd_line.merge_path_cmd != NULL)
			execve(cmd_line.merge_path_cmd, cmd_line.cmd_and_args, info->envp);
		printf("bash: %s: command not found\n", cmd_line.cmd_and_args[0]);
		exit (EXIT_FAILURE);
	}
	waitpid(pid, NULL, 0);
}

// 1- Pourquoi lorsque je change le STDIN pour le fd[0], je ne suis pas oblige de devoir remettre le STDIN original?? 
// 	  ou bien je dois mettre ls STDIN original dans la derniere serie de commande seulement a la TOUTE FIN? 
// 2-- Lorsque j'arrive a la derniere serie de commande, quand il y a eu au moins 1 pipe, est-ce que j'execute dans parent ou child? 
void	execution(t_info *info, t_command_line *line)
{
	t_command_line	cmd_line;
	pid_t			pid[NB_PROCESS];
	int				i;
	
	i = 0;
	cmd_line = line[i];
	info->initial_stdin = dup(STDIN_FILENO);
	info->initial_stdout = dup(STDOUT_FILENO);
	if (info->nb_of_pipe == 0)
		one_command_or_builtin(cmd_line, info);
	else 
	{
		while (i < info->nb_of_pipe)
		{
			cmd_line = line[i];
			dup_redirection(cmd_line); // dup_redirection cause probleme quand redirection dans commandes du milieu
			create_child(cmd_line, info, pid[i]);
			i++;
		}
		i = 0;
		while (i < info->nb_of_pipe)
			waitpid(pid[i++], NULL, 0);
		cmd_line = line[i];
		dup_redirection(cmd_line);
		last_cmd_line(cmd_line, info, pid[i]);	
	}
	put_back_default_std(info);  
}
