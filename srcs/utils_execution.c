/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_execution.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cemenjiv <cemenjiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/15 16:49:06 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/11/18 14:48:45 by cemenjiv         ###   ########.fr       */
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
		check_if_error(cmd_line);
		if (execve(cmd_line.path, cmd_line.argv, info->envp) == -1)
			exit(errno);
	}
	waitpid(pid, &status, 0);
	info->exit_code = get_exit_code(status);
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

//Fonction qui execute une commande avec execve() dans un CHILD
//lorsqu'il n'y a aucun PIPE()
void	one_command_or_builtin(t_command_line *cmd_line, t_info *info)
{
	int	i;

	i = 0;
	do_redirection(cmd_line[i]);
	if (cmd_line[info->index].builtin == 1 && cmd_line->error_infile)
		check_if_error(cmd_line[i]);
	else if (cmd_line[info->index].builtin == 1)
		token_manager(info);
	else
		exec_one_command(cmd_line[info->index], info);
	put_back_default_std(info);
}
