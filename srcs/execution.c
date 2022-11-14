/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cemenjiv <cemenjiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 13:43:50 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/11/14 12:20:33 by cemenjiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h" 

void	exec_error_management(t_command_line cmd_line)
{
	if (cmd_line.cmd_and_args != NULL)
	{
		if (cmd_line.error_infile != NULL)
		{
			write(2, "bash: ", 6);
			write(2, cmd_line.error_infile, ft_strlen(cmd_line.error_infile));
			write(2, ": No such file or directory\n", 28);
		}
		else if (cmd_line.merge_path_cmd == NULL && cmd_line.cmd_and_args[0][0] != '$')
		{
			write(2, "bash: ", 6);
			write(2, cmd_line.cmd_and_args[0], ft_strlen(cmd_line.cmd_and_args[0]));
			write(2, ": command not found\n", 20);
		}
	}
	else if (cmd_line.cmd_and_args == NULL && cmd_line.error_infile != NULL)
	{
		write(2, "bash: ", 6);
		write(2, cmd_line.error_infile, ft_strlen(cmd_line.error_infile));
		write(2, ": No such file or directory\n", 28);
	}
}

void	put_back_default_std(t_info *info)
{
	dup2(info->initial_stdin, STDIN_FILENO);
	close(info->initial_stdin);
	dup2(info->initial_stdout, STDOUT_FILENO);
	close(info->initial_stdout);
}

//Fonction that changes STDIN and STDOUT with dup2()in PARENT before entering 
//CHILD process. I need to keep initial STDIN and STDOUT in memoru for later 
void	do_redirection(t_command_line cmd_line)
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

//1- Dans la derniere ligne de commande, il est important de revenir mettre le STDIN_FILENO qui est l,entree du pipe pour
// le STDIN original
void	last_cmd_or_builtin(t_command_line cmd_line, t_info *info, pid_t *pid)
{
	*pid = fork();
	if (*pid == -1)
		return ;
	if (*pid == 0) // Quand je execve , ca ne ferme pas la fonction mais plutot quitte le CHILD> 
	{
		if (cmd_line.builtin == 1)
		{
			token_manager(info);
			exit (EXIT_SUCCESS);
		}
		else if (cmd_line.merge_path_cmd != NULL && cmd_line.error_infile == NULL)
			execve(cmd_line.merge_path_cmd, cmd_line.cmd_and_args, info->envp);
		exec_error_management(cmd_line);
		exit(EXIT_FAILURE);
	}
}

void	create_child(t_command_line cmd_line, t_info *info, pid_t *pid)
{
	int	fd[2]; // Les fd qui seront associe
	
	if (pipe(fd) == -1) // Creer le pipe() 
		return ;
	*pid = fork(); // Creer un fork qui cree un child et un parent process 
	if (*pid == -1)
		return ;
	if (*pid == 0) // Je rentre dans le child process 
	{
		if (cmd_line.fd_out == 1)
			dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		close(fd[0]);
		if (cmd_line.builtin == 1)
		{
			token_manager(info);
			exit (EXIT_SUCCESS);
		}
		else if (cmd_line.merge_path_cmd != NULL && cmd_line.error_infile == NULL)
			execve(cmd_line.merge_path_cmd, cmd_line.cmd_and_args, info->envp);
		exec_error_management(cmd_line);
		exit(EXIT_FAILURE);
	}
	else
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		if (cmd_line.fd_out != 1) // I must precise also that next command is not last one. 
			dup2(info->initial_stdout, STDOUT_FILENO);
	}
}

void	multiple_commands_or_builtins(t_command_line *cmd_line, t_info *info)
{
	pid_t	pid[NB_PROCESS];
	int		i;
	
	i = 0;
	while (i <= info->nb_of_pipe)
	{
		do_redirection(cmd_line[i]);
		if (i == info->nb_of_pipe)
		{
			last_cmd_or_builtin(cmd_line[i], info, &pid[i]);
			break;
		}
		create_child(cmd_line[i], info, &pid[i]);
		i++;
	}
	i = 0;
	while (i <= info->nb_of_pipe)
		waitpid(pid[i++], NULL, 0);
}

//If command is not valid. Verify that something needs to be freed or not? 
void	exec_one_command(t_command_line cmd_line, t_info *info)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return ; 
	if (pid == 0)
	{
		if (cmd_line.merge_path_cmd != NULL && cmd_line.error_infile == NULL)
			execve(cmd_line.merge_path_cmd, cmd_line.cmd_and_args, info->envp);
		exec_error_management(cmd_line);
		exit(EXIT_FAILURE);
	}
	waitpid(pid, NULL, 0);
}

//Fonction qui execute une commande avec execve() dans un CHILD
//lorsqu'il n'y a aucun PIPE()
void	one_command_or_builtin(t_command_line *cmd_line, t_info *info)
{
	int i;
	
	i = 0;
	do_redirection(cmd_line[i]);
	if (cmd_line[i].builtin == 1 && cmd_line->error_infile) 
		exec_error_management(cmd_line[i]);
	else if (cmd_line[i].builtin == 1)
		token_manager(info);
	else
		exec_one_command(cmd_line[i], info);
}

// 1- Pourquoi lorsque je change le STDIN pour le fd[0], je ne suis pas oblige de devoir remettre le STDIN original?? 
// 	  ou bien je dois mettre ls STDIN original dans la derniere serie de commande seulement a la TOUTE FIN? 
// 2-- Lorsque j'arrive a la derniere serie de commande, quand il y a eu au moins 1 pipe, est-ce que j'execute dans parent ou child? 
void	execution(t_info *info, t_command_line *line)
{
	t_command_line	*cmd_line;

	cmd_line = line;
	if (info->nb_of_pipe == 0)
		one_command_or_builtin(cmd_line, info);
	else 
		multiple_commands_or_builtins(cmd_line, info);
	put_back_default_std(info);  
}
