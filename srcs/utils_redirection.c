/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_redirection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slavoie <slavoie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/15 22:10:20 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/11/25 21:18:28 by slavoie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h" 

void	delete_tokens(t_token **list)
{
	t_token	*temp;
	t_token	*prev;
	int		i;
	int		nodes_to_erase;

	i = 0;
	temp = NULL;
	prev = NULL;
	nodes_to_erase = 0;
	if ((*list)->next)
		nodes_to_erase = 1;
	while (i <= nodes_to_erase)
	{
		temp = *list;
		prev = (*list)->prev;
		*list = (*list)->next;
		ft_lstdelone_token(temp, free);
		temp = NULL;
		if (*list)
			(*list)->prev = prev;
		if (prev)
			prev->next = *list;
		i++;
	}
}

int	is_redirection(t_token *list)
{
	if ((ft_strncmp(list->token, "<", 2) == 0) || \
	(ft_strncmp(list->token, "<<", 2) == 0) || \
		(ft_strncmp(list->token, ">", 2) == 0) || \
		(ft_strncmp(list->token, ">>", 2) == 0))
		return (1);
	else
		return (0);
}

void	delete_redirection_tokens(t_token *list_token, t_token **list_addr)
{
	t_token	*list;

	list = list_token;
	while (list)
	{
		if (is_redirection(list) == 1 && list->prev)
			delete_tokens(&list);
		else if (is_redirection(list) == 1 && !list->prev)
		{
			delete_tokens(&list);
			*list_addr = list;
		}
		if (list && is_redirection(list) == 0)
			list = list->next;
	}
}
