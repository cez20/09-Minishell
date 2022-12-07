/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cemenjiv <cemenjiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 14:57:52 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/12/07 12:42:49 by cemenjiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_token	*ft_lstnew_token(char *content)
{
	t_token	*new;

	new = (t_token *)ft_calloc(1, sizeof(*new));
	if (!new)
		return (0);
	new->token = content;
	new->next = NULL;
	new->prev = NULL;
	new->redirection = 0;
	return (new);
}

t_token	*ft_lstlast_token(t_token *lst)
{
	if (!lst)
		return (0);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

void	ft_lstaddback_token(t_token **alst, t_token *new)
{
	t_token	*last;

	last = NULL;
	if (!*alst)
	{
		*alst = new;
		return ;
	}
	last = ft_lstlast_token(*alst);
	new->prev = last;
	last->next = new;
	new->next = NULL;
}

void	trim_space(t_info *info, char *set)
{
	t_token	*token;
	char	*tmp;

	token = info->list_token;
	while (token)
	{
		tmp = token->token;
		token->token = ft_strtrim(token->token, set);
		free(tmp);
		token = token->next;
	}
}

char	*remove_matching_quote(char *str)
{
	char	*new_str;
	int		i;

	new_str = ft_calloc(ft_strlen(str) + 1, sizeof(char));
	i = 0;
	while (*str)
	{
		if (simple_or_double(str) == D_QUOTE)
		{
			while (*str++ && *str != D_QUOTE)
				new_str[i++] = *str;
			if (*str)
				str++;
		}
		else if (simple_or_double(str) == S_QUOTE)
		{
			while (*str++ && *str != S_QUOTE)
				new_str[i++] = *str;
			if (*str)
				str++;
		}
		else
			new_str[i++] = *str++;
	}
	return (new_str);
}
