/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slavoie <slavoie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 15:07:47 by slavoie           #+#    #+#             */
/*   Updated: 2022/11/25 12:03:06 by slavoie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	remove_inside_quote(t_info *info)
{
	char	*str;
	int		i;
	int		str_index;
	int		len;

	len = ft_strlen(info->list_token->token);
	str_index = 0;
	str = ft_calloc(len, sizeof(char));
	i = 0;
	if (info->list_token->flag_quote == 32)
	{ 
		while (info->list_token->token[i])
		{
			if (info->list_token->token[i] != D_QUOTE && info->list_token->token[i] != S_QUOTE)
			{
				str[str_index] = info->list_token->token[i];
				str_index++;
			}
			i++;
		}
		free(info->list_token->token);
		info->list_token->token = str;
	}
}

void	routine_split_token(t_info *info)
{
	int	type_quote;
	static int	nb_token = 0;

	skip_space(info);
	check_chevron(info);
	type_quote = simple_or_double(info->last_position);
	if (nb_token < 1)
		type_quote = 32;
	ft_lstaddback_token(&info->list_token, ft_lstnew_token(search_another_one(info->last_position, type_quote, info)));
	ft_lstlast_token(info->list_token)->flag_quote = type_quote;
	if (nb_token < 1)
		remove_inside_quote(info);
	skip_space(info);
	trim_space(info, " \t\n\r\v");
	nb_token++;
}

void	split_token(char *token, t_info *info)
{
	int	i;

	i = 0;
	info->last_position = token;
	info->command_lines = \
	ft_calloc(info->nb_of_pipe + 1, sizeof(t_command_line));
	init_command_lines(info->command_lines, info);
	while (*info->last_position)
	{
		routine_split_token(info);
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
	lst_print_token(&info->command_lines[i].list_token);
}

void	fill_command_lines(t_info *info)
{
	int	i;

	i = 0;
	while (i <= info->nb_of_pipe)
	{
		info->command_lines[i].command = \
		get_command(info->command_lines[i].list_token);
		info->command_lines[i].args = \
		get_args(info->command_lines[i].list_token);
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
		if ((*str == '\0' || *str == c))// || (c == 32))// && \
		//(*str == 34 || *str == 39 || *str == '|')))
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
