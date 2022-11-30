/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slavoie <slavoie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/29 17:35:47 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/11/30 15:51:16 by slavoie          ###   ########.fr       */
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

int	parse_error(t_info *info)
{
	if (info->err_chevron == 1)
	{
		ft_putstr_fd("bash: syntax error near unexpected token '<'\n", 2);
		info->err_chevron = 1;
		return (0);
	}
	else if (info->err_chevron == 2)
	{
		ft_putstr_fd("bash: syntax error near unexpected token '>'\n", 2);
		info->err_happen = 1;
		return (0);
	}
	else if (info->err_chevron == 3)
	{
		ft_putstr_fd("bash: syntax error near unexpected token '|'\n", 2);
		info->err_happen = 1;
		return (0);
	}
	return (1);
}
