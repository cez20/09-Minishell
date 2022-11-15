/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cemenjiv <cemenjiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/15 16:49:06 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/11/15 16:49:47 by cemenjiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h" 

void	exec_error_management(t_command_line cmd_line)
{
	if (cmd_line.error_infile != NULL)
	{
		ft_putstr_fd("bash: ", 2);
		ft_putstr_fd(cmd_line.error_infile, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		exit (1);
	}
	else if (cmd_line.merge_path_cmd == NULL && cmd_line.cmd_and_args[0][0] != '$' && \
	cmd_line.cmd_and_args == NULL)
	{
		ft_putstr_fd("bash: ", 2);
		ft_putstr_fd(cmd_line.cmd_and_args[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		exit (127);
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