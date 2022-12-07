/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cemenjiv <cemenjiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 15:07:47 by slavoie           #+#    #+#             */
/*   Updated: 2022/12/07 12:41:49 by cemenjiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	fill_command_lines(t_info *info)
{
	int	i;

	i = 0;
	while (i <= info->nb_of_pipe)
	{
		info->command_lines[i].command = \
		get_command(info->command_lines[i].list_token);
		i++;
	}
}

char	*set_start(t_info *info, char c, char **start, char *str)
{
	info->len = 0;
	*start = str;
	if (c == 32 && *str == 32)
	{
		skip_space(info);
		str = info->last_position;
		*start = str;
	}
	else
	{
		str++;
		info->len++;
	}
	return (str);
}

/*
	Cherche la prochaine occurence de "c" et renvoie le token
*/
char	*search_another_one(char *str, char c, t_info *info)
{
	char	*token;
	char	*start;

	str = set_start(info, c, &start, str);
	while (*str != c)
	{
		if ((*str == '\0' || *str == c) || \
		(c == 32 && (*str == S_QUOTE || *str == D_QUOTE)))
		{
			info->last_position = str;
			token = ft_substr(start, 0, info->len);
			return (token);
		}
		str++;
		info->len++;
	}
	if (c != 32)
	{
		info->last_position = ++str;
		info->len++;
	}
	else
		info->last_position = str;
	token = ft_substr(start, 0, info->len);
	return (token);
}

int	search_next_chevron(t_info *info, int i)
{
	i++;
	if (info->last_position[i] == '<' && !info->last_position[i + 1])
		return (1);
	else if (info->last_position[i] == '>' && !info->last_position[i + 1])
		return (1);
	else if (!info->last_position[i])
		return (1);
	return (0);
}
