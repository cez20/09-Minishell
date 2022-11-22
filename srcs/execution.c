/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cemenjiv <cemenjiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 13:43:50 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/11/22 17:48:08 by cemenjiv         ###   ########.fr       */
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
	{
		info->exit_code = 1;
		exit(info->exit_code);
	}
}

//1- Dans la derniere ligne de commande, il est important 
//de revenir mettre le STDIN_FILENO qui est l,entree du pipe
// pour le STDIN original
// void	last_child_process(t_command_line cmd_line, t_info *info, pid_t *pid)
// {
// 	*pid = fork();
// 	if (*pid == -1)
// 		return ;
// 	if (*pid == 0)
// 	{
// 		do_redirection(cmd_line, info);
// 		do_execution(cmd_line, info);
// 	}	
// 	else
// 	{
// 		if(info->read_pipe != -1)
// 			close(info->read_pipe);
// 		if (cmd_line.fd_in != 0)
// 		{
// 			dup2(cmd_line.fd_in, STDIN_FILENO);
// 			close(cmd_line.fd_in);
// 		}
// 		if (cmd_line.fd_out != 1)
// 		{
// 			dup2(cmd_line.fd_out, STDOUT_FILENO);
// 			close(cmd_line.fd_out);
// 		}
// 	}
// }

// void	child_process(t_command_line cmd_line, t_info *info, pid_t *pid)
// {
// 	int	fd[2];

// 	if (pipe(fd) == -1)
// 		return ;
// 	*pid = fork();
// 	if (*pid == -1)
// 		return ;
// 	if (*pid == 0)
// 	{
// 		do_redirection(cmd_line, info);
// 		if (cmd_line.fd_out == 1)
// 			dup2(fd[1], STDOUT_FILENO);
// 		close(fd[1]);
// 		close(fd[0]);
// 		do_execution(cmd_line, info);
// 	}
// 	else
// 	{
// 		close(fd[1]);
// 		if (info->read_pipe != -1)
// 			close (info->read_pipe);
// 		info->read_pipe = fd[0];
// 	}
// }


// void	multiple_commands_or_builtins(t_command_line *cmd_line, t_info *info)
// {
// 	pid_t	pid[info->nb_of_pipe + 1];
// 	int		status;
// 	int		i;

// 	i = 0;
// 	while (info->index <= info->nb_of_pipe)
// 	{
// 		if (info->index == info->nb_of_pipe)
// 		{
// 			last_child_process(cmd_line[info->index], info, &pid[info->index]);
// 			break ;
// 		}
// 		child_process(cmd_line[info->index], info, &pid[info->index]);
// 		info->index++;
// 	}
// 	while (i <= info->nb_of_pipe)
// 		waitpid(pid[i++], &status, 0);
// 	info->exit_code = get_exit_code(status);
//}
void	close_current_fds(t_command_line *cmd_line, t_info *info)
{
	int i;	
	
	i = info->index;
	if (cmd_line[i].fd_in != 0)
		close (cmd_line[i].fd_in);
	if (cmd_line[i].fd_out != 1)
		close (cmd_line[i].fd_out);
}

void	close_unused_fds(t_command_line *cmd_line, t_info *info)
{
	int i;
	
	i = info->index + 1;

	while (i <= info->nb_of_pipe)
	{
		if (cmd_line[i].fd_in != 0)
			close (cmd_line[i].fd_in);
		if (cmd_line[i].fd_out != 1)
			close (cmd_line[i].fd_out);
		i++;
	}
}

//1- Dans la derniere ligne de commande, il est important 
//de revenir mettre le STDIN_FILENO qui est l,entree du pipe
// pour le STDIN original
void	last_child_process(t_command_line *cmd_line, t_info *info, pid_t *pid)
{
	int i;

	i = info->index;	
	*pid = fork();
	if (*pid == -1)
		return ;
	if (*pid == 0)
	{
		close_unused_fds(cmd_line, info);
		do_redirection(cmd_line[i], info);
		do_execution(cmd_line[i], info);
	}	
	else
	{
		if(info->read_pipe != -1)
			close(info->read_pipe);
		close_current_fds(cmd_line, info);
	}
}

void	child_process(t_command_line *cmd_line, t_info *info, pid_t *pid)
{
	int	fd[2];
	int i;

	i = info->index;
	if (pipe(fd) == -1)
		return ;
	*pid = fork();
	if (*pid == -1)
		return ;
	if (*pid == 0)
	{
		close_unused_fds(cmd_line, info);
		do_redirection(cmd_line[i], info);
		if (cmd_line[i].fd_out == 1)
			dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		close(fd[0]);
		do_execution(cmd_line[i], info);
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
	info->exit_code = get_exit_code(status);
}



// J'ai enelver la fonction put_back_default_std
// et je les ai mise dans chacune des fonctions
// one_command_or_builtin && multiples_commands 
void	execution(t_info *info, t_command_line *line)
{
	t_command_line	*cmd_line;

	cmd_line = line;
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
