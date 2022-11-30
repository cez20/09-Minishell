/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cemenjiv <cemenjiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 16:48:24 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/11/30 14:50:58 by cemenjiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	check_if_error(t_command_line cmd_line, t_info *info)
{
	// printf("error_infile = %s\n", cmd_line.error_infile);
	if (info->heredoc == 1) // Gere les cas de heredoc qui ont ete quitte avec SIGINT 
	{
		if (cmd_line.fd_in != 0)
			close(cmd_line.fd_in);
		exit (1);
	}
	else if (cmd_line.error_infile)
		no_file(cmd_line.error_infile);
	else if (cmd_line.error_outfile)
		no_file(cmd_line.error_outfile);
	else if (!cmd_line.list_token && cmd_line.chevron == 1) // Gerer le cas des redirections sans commande (Ex: << test.txt)
		exit(EXIT_SUCCESS);
	else if (!cmd_line.path && cmd_line.argv[0][0] \
	!= '$' && cmd_line.builtin != 1)  // Quel cas 
		command_not_found(cmd_line.argv[0]);
	else if (cmd_line.argv[0][0] == '$' && ft_strlen(cmd_line.argv[0]) == 1) // Gerer le cas du '$' seul. 
		command_not_found(cmd_line.argv[0]);
	else if (!cmd_line.argv && cmd_line.fd_in > 0)
		exit (EXIT_FAILURE);
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
