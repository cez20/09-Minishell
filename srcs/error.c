/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cemenjiv <cemenjiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 16:48:24 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/11/29 12:43:37 by cemenjiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	check_if_error(t_command_line cmd_line, t_info *info)
{
	if (!cmd_line.list_token)
		syntax_error();
	else if (cmd_line.error_infile)
		no_file(cmd_line.error_infile);
	else if ((ft_strncmp(cmd_line.argv[0], "<\\>", 4) == 0))
		no_file(">");
	else if ((ft_strncmp(cmd_line.argv[0], "<>", 3) == 0))
		syntax_error();
	else if (!cmd_line.path && cmd_line.argv[0][0] \
	!= '$' && cmd_line.builtin != 1)
		command_not_found(cmd_line.argv[0]);
	else if (cmd_line.argv[0][0] == '$' && ft_strlen(cmd_line.argv[0]) == 1)
		command_not_found(cmd_line.argv[0]);
	else if (!cmd_line.argv && cmd_line.fd_in > 0)
		exit (1);
	else if (info->herestring == 1)
		exit (1);
}

void	syntax_error(void)
{
	ft_putstr_fd("bash: syntax error near unexpected token `newline'\n", 2);
	exit (258);
}

void	no_file(char *str)
{
	ft_putstr_fd("bash: ", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd(": No such file or directory\n", 2);
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
	free_info(info);
	if (flag)
	{
		printf("Minishell$> ");
		exit (exit_code);
	}
	else
	{
		printf("exit\n");
		if (exit_code == -42)
			return (0);
		exit(exit_code);
	}
}
