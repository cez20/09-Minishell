/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cemenjiv <cemenjiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 13:43:50 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/11/04 16:29:44 by cemenjiv         ###   ########.fr       */
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

void	exec_multiple_commands(t_command_line cmd_line, t_info *info, int i)
{
	int	fd_pipe[2];
	int pid;

	if (pipe(fd_pipe) == -1) // Creates a pipe 
		return ;
	pid = fork();
	if (pid == -1) 
		return ;
	if (pid == 0 && (i % 2 == 0)) // We want to use the read end of pipe 
	{
		close (fd_pipe[0]); // Closing the read end of pipe 
		dup2(fd_pipe[1], STDOUT_FILENO); // Standard output va aller dans le pipe 
		if (cmd_line.merge_path_cmd != NULL)
			execve(cmd_line.merge_path_cmd, cmd_line.cmd_and_args, info->envp);
		printf("bash: %s: command not found\n", cmd_line.cmd_and_args[0]);
		//free_memory(*pipex);
		exit (127);
	}
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
		while (i <= info->nb_of_pipe)
		{
			cmd_line = line[i];
			dup_redirection(cmd_line, info);
			exec_multiple_commands(cmd_line, info, i);
			i++;
		}
	}
}
