/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cemenjiv <cemenjiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 13:43:50 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/12/01 20:21:07 by cemenjiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h" 

void	do_execution(t_command_line cmd_line, t_info *info)
{
	if (cmd_line.builtin == 1)
	{
		token_manager(info);
		free_struct_command_line(info);
		free_info(info);
		if (info->exit_code != 1)
			exit(EXIT_SUCCESS);
		else
			exit(EXIT_FAILURE);
	}
	else if (execve(cmd_line.path, cmd_line.argv, info->envp) == -1)
		command_not_found(info, cmd_line.argv[0]);
}

//1- Dans la derniere ligne de commande, il est important 
//de revenir mettre le STDIN_FILENO qui est l,entree du pipe
// pour le STDIN original
void	last_child_process(t_command_line *cmd_line, t_info *info, pid_t *pid)
{	
	*pid = fork();
	if (*pid == -1)
		return ;
	if (*pid == 0)
	{
		close_unused_fds(cmd_line, info);
		check_if_error(cmd_line[info->index], info);
		do_redirection(cmd_line[info->index], info);
		do_execution(cmd_line[info->index], info);
	}	
	else
	{
		if (info->read_pipe != -1)
			close(info->read_pipe);
		close_current_fds(cmd_line, info);
	}
}

void	child_process(t_command_line *cmd_line, t_info *info, pid_t *pid)
{
	int	fd[2];

	if (pipe(fd) == -1)
		return ;
	*pid = fork();
	if (*pid == -1)
		return ;
	if (*pid == 0)
	{
		close_unused_fds(cmd_line, info);
		check_if_error(cmd_line[info->index], info);
		do_redirection(cmd_line[info->index], info);
		if (cmd_line[info->index].fd_out == 1)
			dup2(fd[1], STDOUT_FILENO);
		close_fds(fd);
		//free(pid);
		do_execution(cmd_line[info->index], info);
	}
	else
	{
		close(fd[1]);
		if (info->read_pipe != -1)
			close (info->read_pipe);
		info->read_pipe = fd[0];
		close_current_fds(cmd_line, info);
	}
}

void	multiple_commands_or_builtins(t_command_line *cmd_line, t_info *info)
{
	pid_t	*pid;
	int		status;
	int		i;

	pid = ft_calloc(info->nb_of_pipe + 1, sizeof(pid_t));
	while (info->index <= info->nb_of_pipe)
	{
		if (info->index == info->nb_of_pipe)
		{
			last_child_process(cmd_line, info, &pid[info->index]);
			break ;
		}
		child_process(cmd_line, info, &pid[info->index]);
		info->index++;
	}
	i = 0;
	while (i <= info->nb_of_pipe)
		waitpid(pid[i++], &status, 0);
	free(pid);
	info->exit_code = get_exit_code(status);
}

// J'ai enlever la fonction put_back_default_std
// et je les ai mise dans chacune des fonctions
// one_command_or_builtin && multiples_commands 
void	execution(t_info *info, t_command_line *cmd_line)
{
	quote_remover(&cmd_line);
	if (cmd_line->argv)
	{
		if (ft_strncmp(cmd_line->argv[0], "./minishell", 11) == 0)
			enable_signals_minishell();
		else
			enable_signals();
	}
	if (info->nb_of_pipe == 0)
		one_command_or_builtin(cmd_line, info);
	else
		multiple_commands_or_builtins(cmd_line, info);
}
