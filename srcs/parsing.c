/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slavoie <slavoie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 15:07:47 by slavoie           #+#    #+#             */
/*   Updated: 2022/11/19 21:08:47 by slavoie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	split_token(char *token, t_info *info)
{
	int	i;
	int	type_quote;

	i = 0;
	info->last_position = token;
	info->command_lines = ft_calloc(info->nb_of_pipe + 1, sizeof(t_command_line));
	init_command_lines(info->command_lines, info);
	while (*info->last_position)
	{
		skip_space(info);
		check_chevron(info);
		type_quote = simple_or_double(info->last_position);
		ft_lstaddback_token(&info->list_token, ft_lstnew_token \
		(search_another_one(info->last_position, type_quote, info)));
		ft_lstlast_token(info->list_token)->flag_quote = type_quote;
		skip_space(info);
		trim_space(info, " \t\n\r\v");
		if (*info->last_position == '|')
		{
			info->command_lines[i].list_token = info->list_token;
			info->list_token = NULL;
			info->last_position++;
			i++;
		}
	}
	info->command_lines[i].list_token = info->list_token;
	info->list_token = NULL;
}

char	*get_command(t_token *list_token)
{
	if (!list_token)
		return (NULL);
	remove_quote(list_token);
	return (list_token->token);
}

char	*get_args(t_token *list_token)
{
	char	*args;
	char	*tmp;

	args = NULL;
	if (list_token)
		list_token = list_token->next;
	while (list_token)
	{
		tmp = args;
		args = ft_strjoin(args, list_token->token);
		free(tmp);
		list_token = list_token->next;
	}
	return (args);
}

void	fill_command_lines(t_info *info)
{
	int	i;

	i = 0;
	while (i <= info->nb_of_pipe)
	{
		info->command_lines[i].command = get_command(info->command_lines[i].list_token);
		info->command_lines[i].args = get_args(info->command_lines[i].list_token);
		i++;
	}
}

/*
	Cherche la prochaine occurence de "c" et renvoie le token
*/
char	*search_another_one(char *str, char c, t_info *info)
{
	char	*token;
	char	*start;

	start = str;
	info->len = 0;
	if (c == 32 && *str == 32)
	{
		skip_space(info);
		str = info->last_position;
		start = str;
	}
	else
	{
		str++;
		info->len++;
	}
	while (*str != c)
	{
		if (*str == '\0' || (c == 32 && (*str == 34 || *str == 39 || *str == '|')))
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
