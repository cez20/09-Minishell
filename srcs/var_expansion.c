/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expansion.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cemenjiv <cemenjiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 12:27:16 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/11/09 17:04:59 by cemenjiv         ###   ########.fr       */
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

/* Fonction qui trouve l'expansion dans ENV et appelle une autre fonction
pour changer l'expansion par son contenu*/
void	find_expansion(char **str, char *str1, char *str2, char *str3, char **env)
{
	char	*string;
	int		i;

	i = 0;
	while (env[i] && ft_strnstr(env[i], str2, ft_strlen(str2)) == 0)
		i++;
	if (env[i])
	{
		free(*str);
		*str = NULL;
		if (str1)
		{
			string = ft_strjoin(str1, new_expanded_variable(i, str2, env));
			*str = ft_strjoin(string, str3);
			free(string);
		}
		else
		{
			string = ft_strdup(new_expanded_variable(i, str2, env));
			*str = ft_strjoin(string, str3);
			free(string);
		}
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

void	locate_expansion(char **str, char **env)
{
	int		i;
	char	*str1;
	char	*str2;
	char	*str3;
	char	*str4;

	i = 0;
	str1 = NULL; // Pas olbige
	str2 = NULL; // Pas olbige
	str3 = NULL; // Pas olbige
	str4 = NULL; // Pas olbige
	while ((*str)[i] != '$' && (*str)[i])
		i++;
	if ((*str)[i] == '$')
	{
		if (i > 0)
			str1 = ft_substr(*str, 0, i);
		i++;
		str2 = env_variable(*str, &i);
		str3 = ft_strjoin(str2, "=");
		if (*str + 1)
			str4 = ft_strdup(*str + i);
		find_expansion(str, str1, str3, str4, env);
		free(str1);
		free(str2);
		free(str3);
		free(str4);
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
				locate_expansion(&list->token, info->envp);
			//printf("%s\n", list->token);
			list = list->next;
		}
		i++;
	}
}
