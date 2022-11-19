/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cemenjiv <cemenjiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 13:43:50 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/11/19 13:31:49 by cemenjiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h" 

void	do_execution(t_command_line cmd_line, t_info *info)
{
	check_if_error(cmd_line);
	if (cmd_line.builtin == 1)
	{
		token_manager(info);
		exit (EXIT_SUCCESS);
	}
	else if (execve(cmd_line.path, cmd_line.argv, info->envp) == -1)
		exit(errno);
}

//1- Dans la derniere ligne de commande, il est important 
//de revenir mettre le STDIN_FILENO qui est l,entree du pipe
// pour le STDIN original
void	last_child_process(t_command_line cmd_line, t_info *info, pid_t *pid)
{
	*pid = fork();
	if (*pid == -1)
		return ;
	if (*pid == 0)
		do_execution(cmd_line, info);
	put_back_default_std(info);
}

void	child_process(t_command_line cmd_line, t_info *info, pid_t *pid)
{
	int	fd[2];

	if (pipe(fd) == -1)
		return ;
	*pid = fork();
	if (*pid == -1)
		return ;
	if (*pid == 0)
	{
		if (cmd_line.fd_out == 1)
			dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		close(fd[0]);
		do_execution(cmd_line, info);
	}
	else
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		if (cmd_line.fd_out != 1)
		{
			dup2(info->initial_stdout, STDOUT_FILENO);
			//close (info->initial_stdout);
		}
	}
}

void	multiple_commands_or_builtins(t_command_line *cmd_line, t_info *info)
{
	pid_t	pid[NB_PROCESS];
	int		status;
	int		i;

	i = 0;
	while (info->index <= info->nb_of_pipe)
	{
		do_redirection(cmd_line[info->index]);
		if (info->index == info->nb_of_pipe)
		{
			last_child_process(cmd_line[info->index], info, &pid[info->index]);
			break ;
		}
		child_process(cmd_line[info->index], info, &pid[info->index]);
		info->index++;
	}
	while (i <= info->nb_of_pipe)
		waitpid(pid[i++], &status, 0);
	info->exit_code = get_exit_code(status);
}

// J'ai enelver la fonction put_back_default_std
// et je les ai mise dans chacune des fonctions
// one_command_or_builtin && multiples_commands 
void	execution(t_info *info, t_command_line *line)
{
	t_command_line	*cmd_line;

	cmd_line = line; // Je crois que j'ai pas besoin de ca
	if (ft_strncmp(cmd_line->argv[0], "./minishell", 11) == 0)
		enable_signals_minishell();
	else
		enable_signals();
	if (info->nb_of_pipe == 0)
		one_command_or_builtin(cmd_line, info);
	else
		multiple_commands_or_builtins(cmd_line, info);
}
