/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_execution.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cemenjiv <cemenjiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/15 16:49:06 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/11/23 23:24:02 by cemenjiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h" 

void	put_back_default_std(t_info *info)
{
	dup2(info->initial_stdin, STDIN_FILENO);
	close(info->initial_stdin);
	dup2(info->initial_stdout, STDOUT_FILENO);
	close(info->initial_stdout);
}

//If command is not valid. Verify that something needs to be freed or not? 
void	exec_one_command(t_command_line cmd_line, t_info *info)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		return ;
	if (pid == 0)
	{
		check_if_error(cmd_line, info);
		do_redirection(cmd_line, info);
		if (execve(cmd_line.path, cmd_line.argv, info->envp) == -1)
		{
			info->exit_code = 1;
			exit(info->exit_code);
		}
	}
	close_current_fds(&cmd_line, info);
	waitpid(pid, &status, 0);
	info->exit_code = get_exit_code(status);
}

//Fonction that changes STDIN and STDOUT with dup2()in PARENT before entering 
//CHILD process. I need to keep initial STDIN and STDOUT in memoru for later 
void	do_redirection(t_command_line cmd_line, t_info *info)
{
	if (info->read_pipe != -1)
	{
		dup2(info->read_pipe, STDIN_FILENO);
		close(info->read_pipe);
	}
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
void	one_command_or_builtin(t_command_line *cmd_line, t_info *info)
{
	int	i;

	i = 0;
	if (cmd_line[info->index].builtin == 1 && cmd_line->error_infile)
		check_if_error(cmd_line[i], info);
	else if (cmd_line[info->index].builtin == 1)
	{
		info->initial_stdin = dup(STDIN_FILENO);
		info->initial_stdout = dup(STDOUT_FILENO);
		do_redirection(cmd_line[info->index], info);
		token_manager(info);
		put_back_default_std(info);
	}
	else
		exec_one_command(cmd_line[info->index], info);
}
