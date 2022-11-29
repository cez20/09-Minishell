/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slavoie <slavoie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/15 23:00:39 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/11/29 14:24:47 by slavoie          ###   ########.fr       */
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
	{
		if (lst->token)
			del(lst->token);
	}
	if (lst)
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

void	print_error_cd(char *str)
{
	ft_putstr_fd("cd: ", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd(": No such file or directory\n", 2);
}
