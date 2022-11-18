/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expansion.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cemenjiv <cemenjiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 12:27:16 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/11/15 10:27:09 by cemenjiv         ###   ########.fr       */
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
// char	*chop_chop_$(char **tab)
// {
// 	char *str;

// 	str = ft_strjoin(tab[0], tab[3]);
// 	return (str);
// }

/* Fonction qui trouve l'expansion dans ENV et appelle une autre fonction
pour changer l'expansion par son contenu*/
void	find_expansion(char **str, char **tab, char **env)
{
	char	*string;
	int		i;

	i = 0;
	while (env[i] && ft_strnstr(env[i], tab[2], ft_strlen(tab[2])) == 0)
		i++;
	if (env[i])
	{
		free(*str);
		*str = NULL;
		if (tab[0])
		{
			string = ft_strjoin(tab[0], new_expanded_variable(i, tab[2], env));
			*str = ft_strjoin(string, tab[3]);
			free(string);
		}
		else
		{
			string = ft_strdup(new_expanded_variable(i, tab[2], env));
			*str = ft_strjoin(string, tab[3]);
			free(string);
		}
	}
	else
	{
		string = *str;
		print_tab(tab);
		*str = ft_strjoin(tab[0], tab[3]);
		printf("str =%s\n", *str);
		free(string);
	}
}


/*Fonction qui s'assurer de seulement garder la variable
d'environmement sans metachracteres (EX:"echo'$ARGS'"")
va seulement garder le ARGS sans le '(simple quote qui se trouve apres)*/
char	*env_variable(char *str, int *i)
{
	char	*str1;
	int		j;

	j = *i;
	while (ft_isalpha(str[*i]) != 0)
		(*i)++;
	str1 = ft_substr(str, j, *i - j);
	return (str1);
}


void	locate_expansion(char **str, char **env, t_info *info)
{
	int		i;
	char **tab;

	i = 0;
	tab = ft_calloc(5, sizeof(char*));
	if ((*str)[1] == '?' && ft_strlen(*str) == 2)
	{
		free(*str);
		*str = ft_itoa(info->exit_code);
	}
	while ((*str)[i] != '$' && (*str)[i])
		i++;
	if ((*str)[i] == '$')
	{
		if (i > 0)
			tab[0] = ft_substr(*str, 0, i);
		i++;
		tab[1] = env_variable(*str, &i);
		tab[2] = ft_strjoin(tab[1], "=");
		if (*str + 1)
			tab[3] = ft_strdup(*str + i);
		find_expansion(str, tab, env);
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
			if (ft_strchr(list->token, '$'))
				locate_expansion(&list->token, info->envp, info);
			//printf("%s\n", list->token);
			list = list->next;
		}
		i++;
	}
}
