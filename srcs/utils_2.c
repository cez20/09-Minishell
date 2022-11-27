/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slavoie <slavoie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/15 23:00:39 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/11/27 20:45:29 by slavoie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	lst_print_token(t_token **list)
{
	t_token	*node;
	int		i;

	node = *list;
	i = 0;
	if (!node)
		printf("node[0] is NULL\n");
	while (node)
	{
		if (!node)
			return ;
		printf("node[%d] = %s\n", i, (char *)node->token);
		node = node->next;
		i++;
	}
}

void	ft_lstdelone_token(t_token *lst, void (*del)(void *))
{
	if (del)
		del(lst->token);
	free(lst);
}

int	ft_lstsize_token(t_token *lst)
{
	int	size;

	size = 0;
	while (lst)
	{
		lst = lst->next;
		size++;
	}
	return (size);
}

void	ft_lstclear_token(t_token **lst, void (*del) (void *))
{
	t_token	*elem;
	int		i;

	if (!*lst)
		return ;
	i = ft_lstsize_token(*lst);
	while (i != 0)
	{
		elem = (*lst)->next;
		ft_lstdelone_token(*lst, del);
		*lst = elem;
		i--;
	}
}
