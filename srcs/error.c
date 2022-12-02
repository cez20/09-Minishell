/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cemenjiv <cemenjiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 16:48:24 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/12/02 16:00:46 by cemenjiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	check_if_error(t_command_line cmd_line, t_info *info)
{
	if (info->heredoc == 1)
	{
		if (cmd_line.fd_in != 0)
			close(cmd_line.fd_in);
		free_structs_and_exit(info, EXIT_FAILURE);
	}
	else if (cmd_line.error_infile)
		no_file(info, cmd_line.error_infile);
	else if (cmd_line.error_outfile)
		no_file(info, cmd_line.error_outfile);
	else if (!cmd_line.list_token && cmd_line.chevron == 0)
		syntax_error(info);
	else if (!cmd_line.list_token && cmd_line.chevron == 1)
		free_structs_and_exit(info, EXIT_SUCCESS);
	// else if (!cmd_line.path && cmd_line.argv[0][0] \
	// != '$' && cmd_line.builtin != 1)
	// 	command_not_found(info, cmd_line.argv[0]);
	// else if (cmd_line.argv[0][0] == '$' && ft_strlen(cmd_line.argv[0]) == 1)
	// 	command_not_found(info, cmd_line.argv[0]);
	else if (!cmd_line.argv && cmd_line.fd_in > 0)
		free_structs_and_exit(info, EXIT_FAILURE);
}

void	syntax_error(t_info *info)
{
	ft_putstr_fd("bash: syntax error near unexpected token `newline'\n", 2);
	free_structs_and_exit(info, 258);
}

void	no_file(t_info *info, char *str)
{
	ft_putstr_fd("bash: ", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd(": No such file or directory\n", 2);
	if (info->command_lines[info->index].builtin == 0)
		free_structs_and_exit(info, EXIT_FAILURE);
}

void	command_not_found(t_info *info, char *str)
{
	ft_putstr_fd("bash: ", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd(": command not found\n", 2);
	free_structs_and_exit(info, 127);
}
