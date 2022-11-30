/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slavoie <slavoie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 16:48:24 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/11/30 14:52:11 by slavoie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	check_if_error(t_command_line cmd_line, t_info *info)
{
	// printf("error_infile = %s\n", cmd_line.error_infile);

	if (info->herestring == 1 || info->heredoc == 1)
	{
		if (cmd_line.fd_in != 0)
			close(cmd_line.fd_in);
		exit (1);
	}
	else if (cmd_line.error_infile)
		no_file(info, cmd_line.error_infile);
	// else if (!cmd_line.list_token && cmd_line.chevron == 0)
	// 	syntax_error();
	else if (!cmd_line.list_token && cmd_line.chevron == 1)
		exit(0);
	else if (!cmd_line.path && cmd_line.argv[0][0] \
	!= '$' && cmd_line.builtin != 1)
		command_not_found(cmd_line.argv[0]);
	else if (cmd_line.argv[0][0] == '$' && ft_strlen(cmd_line.argv[0]) == 1)
		command_not_found(cmd_line.argv[0]);
	else if (!cmd_line.argv && cmd_line.fd_in > 0)
	{
		exit (1);
	}
}

void	syntax_error(void) 
{
	ft_putstr_fd("bash: syntax error near unexpected token `newline'\n", 2);
	exit (258);
}

void	no_file(t_info *info, char *str)
{
	ft_putstr_fd("bash: ", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd(": No such file or directory\n", 2);
	if (info->nb_of_pipe > 0)
		exit (1);
}

void	command_not_found(char *str)
{
	ft_putstr_fd("bash: ", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd(": command not found\n", 2);
	exit (127);
}

//I need to free eveything that cause a segfault 
int	exit_terminal(t_info *info, int flag, int exit_code)
{
	close(info->initial_stdin);
	close (info->initial_stdout);
	if (flag)
	{
		printf("\033[1A\001"GREEN"\002Minishell\001"RESET"\002$> exit\n");
		exit (exit_code);
	}
	else
	{
		if (exit_code == -42)
			return (1);
		free_struct_command_line(info);
		free_info(info);
		printf("exit\n");
		exit(exit_code);
	}
}
