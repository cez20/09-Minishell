/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expansion.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cemenjiv <cemenjiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/26 09:58:57 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/09/30 15:20:50 by cemenjiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Libft/libft.h"


/* Gerer les cas d'erreurs:
- (FAIT) Si le premier caractere du token n'est pas '$', ne rien faire. Le token, reste tel quel.
- (FAIT) Si le contenu suivant n'est pas exactement la meme expression (ex: ARG au lieu de ARGS), on retourne le token initial
- (FAIT) Si le contenu suivant le caractere '$' ne se trouve pas dans l'environnement, ne rien faire et retourner le token initial
- (FAIT) Si l'expansion est associe a null = ARGS = "", remplacer $ARGS par null;
- Si l'expansion ne se trouve pas dans ENV, output sera RIEN. Ex: echo $ARGS si ARGS n'est pas dans env. Output est rien.
- Si le contenu est $? = retourner l'exit number. Dans tous les autres cas, ne rien faire.  
*/

char	*new_expanded_variable(int i, char *str, char **env)
{
	int j;

	j = 0;
	while (env[i][j] == str[j] && str[j])
		j++;
	str = &env[i][j];
	return (str);
}

// char	*var_expansion(char **str, char **env)
// {
// 	char *str1; // string qui contient contenu avant le dollar sign 
// 	char *str2; // string qui contient contenu apres le dollar sign 
// 	char *str3; // String qui contient contient contenu apres dollar sign + "="
// 	int i;

// 	i = 0;
// 	while ((*str)[i] != '$')
// 		i++;
// 	if (i > 0)
// 		str3 = ft_substr(*str, 0, i);
// 	if ((*str)[i] == '$')	
// 	{
// 		i++;
// 		str1 = ft_strdup((*str + i));
// 		str2 = ft_strjoin(str1, "=");
// 		i = 0;
// 		while (env[i] != NULL && ft_strnstr(env[i], str2, ft_strlen(str2)) == 0)
// 		  	i++;
// 		printf("La variable i est d'une valeur de : %d\n", i);
// 		if (env[i] != NULL)
// 		{
// 			if (str3)
// 			{
// 				free(*str);	
// 				*str = ft_strjoin(str3, new_expanded_variable(i, str2, env));
// 			}
// 			else
// 				*str = new_expanded_variable(i, str2, env);
// 		}
// 		free(str1);
// 		free(str2);
// 		free(str3);
// 		printf("Le token final une fois l'expansion fait est: %s\n", *str);
// 	}
// 	return (*str);
// }

void	remplace_string(char *str, char *str1, char *str2, char **env)
{
	int i;

	i = 0;
	while (env[i] != NULL && ft_strnstr(env[i], str2, ft_strlen(str2)) == 0)
		i++;
	if (env[i] != NULL)
	{
		if (str1)
		{
			free(str);	
			str = ft_strjoin(str1, new_expanded_variable(i, str2, env));
		}
		else
			str = new_expanded_variable(i, str2, env);
	}
	printf("Le token final une fois l'expansion fait est: %s\n", str);
}


char	*var_expansion(char **str, char **env)
{
	char *str1; // string qui contient contenu avant le dollar sign 
	char *str2; // string qui contient contenu apres le dollar sign 
	char *str3; // String qui contient contient contenu apres dollar sign + "="
	int i;

	i = 0;
	str1 = NULL;
	while ((*str)[i] != '$' && (*str)[i])
		i++;
	if ((*str)[i] == '$')	
	{
		if (i > 0)
			str1 = ft_substr(*str, 0, i);
		i++;
		str2 = ft_strdup((*str + i));
		str3 = ft_strjoin(str2, "=");
		remplace_string(*str, str1, str3, env);
		free(str1);
		free(str2);
		free(str3);
	}
	return (*str);
}


int main(int argc, char **argv, char **env)
{
	(void)argc;
	(void)argv;
	char *str;
	
	str = malloc(10 * sizeof(char));
	strcpy(str, "echoARGS");
	var_expansion(&str, env);
	printf("Le token final est: %s\n", str);
	free(str); // Seulement si la premiere lettre du string n'est pas $
}