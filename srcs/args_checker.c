/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_checker.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cemenjiv <cemenjiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 17:42:30 by slavoie           #+#    #+#             */
/*   Updated: 2022/11/15 16:37:37 by cemenjiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

 int	check_arg_export(char *arg)
{
	int	i;

	i = 0;
	if (arg[i] == '=')
	{
		printf("bash: export: '%s': not a valid identifier\n", arg);
		return (0);
	}
	while (arg[i])
	{
		if (arg[i] == '=')
			return (1);
		i++;
	}
	return (0);
}

int	check_arg_unset(char *arg, t_info *info)
{
	int	i;

	i = 0;
	while (arg[i])
	{
		if (arg[i] == '=')
		{
			printf("bash: unset: '%s': not a valid identifier\n", arg);
			info->exit_code = 1;
			return (0);
		}
		i++;
	}
	return (1);
}