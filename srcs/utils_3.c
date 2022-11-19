/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slavoie <slavoie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 15:04:58 by slavoie           #+#    #+#             */
/*   Updated: 2022/11/18 19:29:00 by slavoie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*
	Vérifier la valeur du char à l'adresse du char pointeur "token"
	renvoie la valeur du char si c'est '' ou ""
	sinon renvoie la valeur de l'espace soit '32' 
*/
char	simple_or_double(char *token)
{
	if (*token == 34 || *token == 39)
		return (*token);
	return (32);
}

/*
    compte le nombre d'élément correspondant au char c dans la string
*/
int	how_many(t_info *info, char *str, char c)
{
	int	i;

	i = 0;
	while (str && *str)
	{
		if (*str == D_QUOTE)
		{
			info->state = D_QUOTE;
			str++;
			while (*str && *str != D_QUOTE)
				str++;
			str++;
		}
		if (*str == S_QUOTE)
		{
			info->state = S_QUOTE;
			str++;
			while (*str && *str != S_QUOTE)
				str++;
			str++;
		}
		if (*str == c)
			i++;
		str++;
	}
	return (i);
}

/*
	Avance le pointeur jusqu'à la prochaine chose à interpréter
*/
void	skip_space(t_info *info)
{
	if (*info->last_position)
	{
		while (is_white_space(*info->last_position))
		{
			if (*info->last_position == '|')
				return ;
			info->last_position++;
			info->len++;
			if (info->list_token)
				ft_lstlast_token(info->list_token)->space_flag = 1;
		}
	}
}

char	**split_path(char **env)
{
	char	**str;
	int		i;

	i = 0;
	while (env[i])
	{
		if (ft_strnstr(env[i], "PATH=", 5))
		{
			str = ft_split(&env[i][5], ':');
			return (str);
		}
		i++;
	}
	return (NULL);
}
