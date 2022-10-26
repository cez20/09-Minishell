/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cemenjiv <cemenjiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 15:04:58 by slavoie           #+#    #+#             */
/*   Updated: 2022/10/26 18:03:18 by cemenjiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	Vérifier la valeur du char à l'adresse du char pointeur "token"
	renvoie la valeur du char si c'est '' ou ""
	sinon renvoie la valeur de l'espace soit '32' 
*/
char simple_or_double(char *token)
{
	if (*token == 34 || *token == 39)
		return (*token);
	return (32);
}

/*
    compte le nombre d'élément correspondant au char c dans la string
*/
int	how_many(char *str, char c)
{
	int i;

	i = 0;
	while (*str)
	{
		if (*str == c)
			i++;
		str++;
	}
	return (i);
}

/*
	Avance le pointeur jusqu'à la prochaine chose à interpréter
*/
void skip_space(t_info *info)
{
	if (*info->last_position)
	{

		while(is_white_space(*info->last_position))
		{
			if (*info->last_position == '|')
				return ;
			info->last_position++;
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

void	token_deletion(t_token *list)
{	
	// t_token	 *prev_node;
	// t_token	 *next_node;

	// prev_node = list->prev;
	// next_node = list->next;

	// printf("The prev_node address is %p\n", prev_node);
	// printf("The next_node address is %p\n", next_node);
	// list->token = NULL;
	// list->next->token = NULL;

	free(list->token);

	//Soit je prends la chaine intiale info->lis_token au lieu de info->commands[i]->list_token e
	// et j'enleve les nodes qui s'y trouvent, ou bien sinon je malloc une nouvelle liste auquel j'assigne 
}