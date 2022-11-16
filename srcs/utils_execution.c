/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_execution.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cemenjiv <cemenjiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/15 16:49:06 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/11/16 00:16:05 by cemenjiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h" 





void	check_if_error(t_command_line cmd_line)
{
	if (cmd_line.error_infile != NULL)
	{
		ft_putstr_fd("bash: ", 2);
		ft_putstr_fd(cmd_line.error_infile, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		exit (1);
	}
	else if (cmd_line.path == NULL && cmd_line.argv[0][0] != '$' && \
	cmd_line.argv == NULL)
	{
		ft_putstr_fd("bash: ", 2);
		ft_putstr_fd(cmd_line.argv[0], 2);
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

//If command is not valid. Verify that something needs to be freed or not? 
void	exec_one_command(t_command_line cmd_line, t_info *info)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return ;
	if (pid == 0)
	{
		check_if_error(cmd_line);
		if (execve(cmd_line.path, cmd_line.argv, info->envp) == -1)
			exit(errno);
	}
	waitpid(pid, &info->exit_code, 0);
	if (WIFEXITED(info->exit_code))
		info->exit_code = WEXITSTATUS(info->exit_code);
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
	if (cmd_line[i].builtin == 1 && cmd_line->error_infile)
		check_if_error(cmd_line[i]);
	else if (cmd_line[i].builtin == 1)
		token_manager(info);
	else
		exec_one_command(cmd_line[i], info);
}