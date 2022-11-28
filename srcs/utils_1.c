/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slavoie <slavoie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 14:57:52 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/11/28 17:29:26 by slavoie          ###   ########.fr       */
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

void	remove_inside_quote(t_info *info)
{
	char	*str;
	int		i;
	int		str_index;
	int		len;

	len = ft_strlen(info->list_token->token);
	str_index = 0;
	i = 0;
	if (info->list_token->flag_quote == 32)
	{
		str = ft_calloc(len + 1, sizeof(char));
		while (info->list_token->token[i])
		{
			if (info->list_token->token[i] != D_QUOTE && \
			info->list_token->token[i] != S_QUOTE)
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
