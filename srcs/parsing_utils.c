/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slavoie <slavoie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/20 16:14:11 by stevenlavoi       #+#    #+#             */
/*   Updated: 2022/11/28 12:37:30 by slavoie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*check_double_quote(char *str, t_info *info)
{
	if (*str == D_QUOTE)
	{
		info->state = D_QUOTE;
		while (str && *str)
		{
			str++;
			if (*str && *str == D_QUOTE)
			{
				info->state = TEXT;
				return (str);
			}
		}
	}
	return (str);
}

char	*check_simple_quote(char *str, t_info *info)
{
	if (*str == S_QUOTE)
	{
		info->state = S_QUOTE;
		while (str && *str)
		{
			str++;
			if (*str && *str == S_QUOTE)
			{
				info->state = TEXT;
				return (str);
			}
		}
	}
	return (str);
}

int	close_quote_checker(t_info *info, char *str)
{
	info->state = TEXT;
	while (str && *str)
	{
		str = check_double_quote(str, info);
		str = check_simple_quote(str, info);
		if (*str)
			str++;
	}
	if (info->state == TEXT)
		return (1);
	else
		return (0);
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

void	ft_str_tolower(char *str)
{
	while (*str)
	{
		*str = ft_tolower(*str);
		str++;
	}
}

char	*get_command(t_token *list_token)
{
	if (!list_token)
		return (NULL);
	// remove_quote(list_token);
	ft_str_tolower(list_token->token);
	return (list_token->token);
}
