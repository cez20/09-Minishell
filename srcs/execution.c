/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cemenjiv <cemenjiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 13:43:50 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/11/15 22:24:40 by cemenjiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h" 

//1- Dans la derniere ligne de commande, il est important de revenir mettre le STDIN_FILENO qui est l,entree du pipe pour
// le STDIN original
void	last_process(t_command_line cmd_line, t_info *info, pid_t *pid)
{
	*pid = fork();
	if (*pid == -1)
		return ;
	if (*pid == 0)
	{
		exec_error_management(cmd_line);
		if (cmd_line.builtin == 1)
		{
			token_manager(info);
			exit (EXIT_SUCCESS);
		}
		else if (cmd_line.merge_path_cmd != NULL && cmd_line.error_infile == NULL)
			execve(cmd_line.merge_path_cmd, cmd_line.cmd_and_args, info->envp);
		exit(EXIT_FAILURE);
	}
}

void	create_child(t_command_line cmd_line, t_info *info, pid_t *pid)
{
	int	fd[2];

	if (pipe(fd) == -1)
		return ;
	*pid = fork();
	if (*pid == -1)
		return ;
	if (*pid == 0)
	{
		exec_error_management(cmd_line);
		if (cmd_line.fd_out == 1)
			dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		close(fd[0]);
		if (cmd_line.builtin == 1)
		{
			token_manager(info);
			exit (EXIT_SUCCESS);
		}
		else if (execve(cmd_line.merge_path_cmd, cmd_line.cmd_and_args, info->envp) == -1)
			exit(errno);
	}
	else
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		if (cmd_line.fd_out != 1)
			dup2(info->initial_stdout, STDOUT_FILENO);
	}
}

void	multiple_commands_or_builtins(t_command_line *cmd_line, t_info *info)
{
	pid_t	pid[NB_PROCESS];
	int		i;

	i = 0;
	while (info->index <= info->nb_of_pipe)
	{
		do_redirection(cmd_line[info->index]);
		if (info->index == info->nb_of_pipe)
		{
			last_process(cmd_line[info->index], info, &pid[info->index]);
			break ;
		}
		create_child(cmd_line[info->index], info, &pid[info->index]);
		info->index++;
	}
	while (i <= info->nb_of_pipe)
		waitpid(pid[i++], &info->exit_code, 0);
	if (WIFEXITED(info->exit_code))
		info->exit_code = WEXITSTATUS(info->exit_code);
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
