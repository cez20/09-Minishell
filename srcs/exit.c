/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cemenjiv <cemenjiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/29 17:35:47 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/11/29 17:37:59 by cemenjiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

//Fonction qui intercepte exit_code:
// Macro WIFEXITED retourne true si child a quitte correctement
// Macro WEXITSTATUS permet de transformer le status dans le bon int
// Macro WIFSIGNALED permet de capter si child a quitte avec signal
// WTERMSIG transforme status bon int. On ajoute 128 pour avoir le bon exit_code
int	get_exit_code(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (0);
}

void	if_exit_code(char **str, t_info *info)
{
	if ((*str)[1] == '?' && ft_strlen(*str) == 2)
	{
		free(*str);
		*str = ft_itoa(info->exit_code);
	}
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
		free_struct_command_line(info);
		free_info(info);
		printf("exit\n");
		exit(exit_code);
	}
}
