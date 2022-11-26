/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_checker.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slavoie <slavoie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 17:42:30 by slavoie           #+#    #+#             */
/*   Updated: 2022/11/25 20:18:21 by slavoie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	check_arg_export(char *arg, t_info *info)
{
	int	i;

	i = 0;
	if (!ft_isalpha(arg[i]) && arg[i] != '_')
	{
		ft_putstr_fd("bash: export: '", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		info->exit_code = 1;
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
			ft_putstr_fd("bash: unset: '", 2);
			ft_putstr_fd(arg, 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			// printf("bash: unset: '%s': not a valid identifier\n", arg);
			info->exit_code = 1;
			return (0);
		}
		i++;
	}
	return (1);
}

void	input_chevron(t_info *info, int i)
{
	if (info->last_position[i + 2] == '<')
	{
		printf("bash: syntax error near unexpected token '<'\n");
		info->herestring = 1;
	}
	else if (info->last_position[i + 1] == '<')
	{
		ft_lstaddback_token(&info->list_token, \
		ft_lstnew_token(ft_substr(info->last_position, i, 2)));
		info->last_position++;
		info->last_position++;
	}
	else if (!(info->last_position[i + 1] == '<'))
	{
		ft_lstaddback_token(&info->list_token, \
		ft_lstnew_token(ft_substr(info->last_position, i, 1)));
		info->last_position++;	
	}
}

void	output_chevron(t_info *info, int i)
{
	if (info->last_position[i + 2] == '>')
		printf("bash: syntax error near unexpected token '>'\n");
	else if (info->last_position[i + 1] == '>')
	{
		ft_lstaddback_token(&info->list_token, \
		ft_lstnew_token(ft_substr(info->last_position, i, 2)));
		info->last_position++;
		info->last_position++;
	}
	else if (!(info->last_position[i + 1] == '>'))
	{
		ft_lstaddback_token(&info->list_token, \
		ft_lstnew_token(ft_substr(info->last_position, i, 1)));
		info->last_position++;	
	}
}

void	check_chevron(t_info *info)
{
	int	i;

	i = 0;
	if (info->last_position[i] == '<')
		input_chevron(info, i);
	else if (info->last_position[i] == '>')
		output_chevron(info, i);
}
