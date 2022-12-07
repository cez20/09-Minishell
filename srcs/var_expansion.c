/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expansion.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slavoie <slavoie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 12:27:16 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/12/06 16:16:13 by slavoie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/* Fonction qui circule dans ENV, et retourn le contenu qui se trouve
apres le signe (=);*/
char	*new_expanded_variable(int i, char *str, char **env)
{
	char	*str1;
	int		j;

	j = 0;
	while ((env[i][j] == str[j]) && str[j])
		j++;
	str1 = &env[i][j];
	return (str1);
}

/*Fonction qui s'assurer de seulement garder la variable
d'environmement sans metachracteres (EX:"echo'$ARGS'"")
va seulement garder le ARGS sans le '(simple quote qui se trouve apres)*/
char	*env_variable(char *str, int *i)
{
	char	*str1;
	int		j;

	j = *i;
	while (ft_isalnum(str[*i]) != 0)
		(*i)++;
	str1 = ft_substr(str, j, *i - j);
	return (str1);
}


void	locate_expansion(char **str, char **env, t_info *info)
{
	char	**tab;
	int		i;
	int		count;

	if_exit_code(str, info);
	i = 0;
	count = 0;
	while ((*str)[i])
	{
		if ((*str)[i] == '$' && (*str)[i + 1])
		{
			tab = ft_calloc(5, sizeof(char *));
			if (i > 0)
				tab[0] = ft_substr(*str, 0, i);
			i++;
			tab[1] = env_variable(*str, &i);
			tab[2] = ft_strjoin(tab[1], "=");
			if (*(*str + i))
				tab[3] = ft_strdup(*str + i);
			find_expansion(str, tab, env);
			free_tab(tab);
			i = -1;
		}
		i++;
	}
}

/* A valider si on garde le 2e if qui enleve les quotes */
void	var_expansion(t_command_line *cmd_line, t_info	*info)
{
	t_token	*list;
	int		i;

	i = 0;
	list = NULL;
	while (i < (info->nb_of_pipe + 1))
	{
		list = cmd_line[i].list_token;
		while (list)
		{
			if (list->flag_quote != S_QUOTE)
			{
				if (ft_strchr(list->token, '$'))
					locate_expansion(&list->token, info->envp, info);
			}
			list = list->next;
		}
		i++;
	}
}
