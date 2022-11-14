/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_checker.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slavoie <slavoie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 17:42:30 by slavoie           #+#    #+#             */
/*   Updated: 2022/11/14 17:56:42 by slavoie          ###   ########.fr       */
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

int	check_arg_unset(char *arg)
{
	int	i;

	i = 0;
	while (arg[i])
	{
		if (arg[i] == '=')
		{
			printf("bash: unset: '%s': not a valid identifier\n", arg);
			return (0);
		}
		i++;
	}
	return (1);
}