/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stevenlavoie <stevenlavoie@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/20 16:14:11 by stevenlavoi       #+#    #+#             */
/*   Updated: 2022/12/03 22:50:24 by stevenlavoi      ###   ########.fr       */
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
	ft_str_tolower(list_token->token);
	return (list_token->token);
}
