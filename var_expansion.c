/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expansion.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cemenjiv <cemenjiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/26 09:58:57 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/10/10 12:24:16 by cemenjiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include <string.h>

/* Gerer les cas d'erreurs:
- (FAIT) Si le premier caractere du token n'est pas '$', ne rien faire. Le token, reste tel quel.
- (FAIT) Si le contenu suivant n'est pas exactement la meme expression (ex: ARG au lieu de ARGS), on retourne le token initial
- (FAIT) Si le contenu suivant le caractere '$' ne se trouve pas dans l'environnement, ne rien faire et retourner le token initial
- (FAIT) Si l'expansion est associe a null = ARGS = "", remplacer $ARGS par null;
- Si l'expansion ne se trouve pas dans ENV, output sera RIEN. Ex: echo $ARGS si ARGS n'est pas dans env. Output est rien.
- Si le contenu est $? = retourner l'exit number. Dans tous les autres cas, ne rien faire.  
*/
int	ft_isalpha1(int c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
		return (1);
	else
		return (0);
}

/* Fonction qui circule dans ENV, et retourn le contenu qui se trouve
apres le signe (=);*/
char	*new_expanded_variable(int i, char *str, char **env)
{
	int	j;

	j = 0;
	while (env[i][j] == str[j] && str[j])
		j++;
	str = &env[i][j];
	return (str);
}

/* Fonction qui trouve l'expansion dans ENV et appelle une autre fonction
pour changer l'expansion par son contenu
1- Je circule dans env, jusqu'a trouver la bonne ligne correspondant a expansion
2- Une fois, dans la bonne ligne, je rentre dans une condition:
	a) S'il y un str1(contenu avec) expansion, str va pointer un string qui joint
		str1 et le contenu de la variable d'expansion
	b) Sinon, str va pointer vers le contenu de la variable d'expansion */
void	find_expansion(char **str, char *str1, char *str2, char **env)
{
	int	i;

	i = 0;
	while (env[i] && ft_strnstr(env[i], str2, ft_strlen(str2)) == 0)
		i++;
	if (env[i])
	{
		free(*str);
		if (str1)
			*str = ft_strjoin(str1, new_expanded_variable(i, str2, env));
		else
			*str = ft_strdup(new_expanded_variable(i, str2, env));
	}
}

// Fonction qui traire l'expansion avec dollar sign($)
// 1- On circule dans la string jusqu'a renconter un $. 
// 2- Si applicable, on copie le contenu de la string anterieur 
// au dollar sign (ex: echo$ARGS copiera "echo") dans str1
// 3- On copie le contenu se trouvant apres le dollar sign dans un nouvelle
// 	string (str2) qui est malloc avec strdup
// 4- On creer une nouvelle string en fusionant str2(ARGS) et "=" 
//    pour donner "ARGS="
// 5- J'envoie le tout a la fonction find_expansion qui fera la substition 
// char	*var_expansion(char **str, char **env)
// {
// 	char	*str1;
// 	char	*str2;
// 	char	*str3;
// 	int		i;

// 	i = 0;
// 	str1 = NULL;
// 	while ((*str)[i] != '$' && (*str)[i])
// 		i++;
// 	if ((*str)[i] == '$')
// 	{
// 		if (i > 0)
// 			str1 = ft_substr(*str, 0, i);
// 		i++;
// 		str2 = ft_strdup((*str + i));
// 		str3 = ft_strjoin(str2, "=");
// 		find_expansion(str, str1, str3, env);
// 		free(str1);
// 		free(str2);
// 		free(str3);
// 	}
// 	return (*str);
///

char *new_string(char *str, int *i)
{
	char *str1;
	int j;

	j = *i;
	while (ft_isalpha1(str[*i]) != 0)
		(*i)++;
	str1 = ft_substr(str, j, *i - j);
	printf("The new string is %s\n", str1);
	return (str1);
}


char	*var_expansion(char **str, char **env)
{
	char	*str1;
	char	*str2;
	char	*str3;
	char	*str4;
	int		i;

	i = 0;
	str1 = NULL;
	while ((*str)[i] != '$' && (*str)[i])
		i++;
	if ((*str)[i] == '$')
	{
		if (i > 0)
			str1 = ft_substr(*str, 0, i);
		i++;
		str2 = new_string(*str, &i);
		str3 = ft_strjoin(str2, "=");
		str4 = ft_strdup(*str + i);
		find_expansion(str, str1, str3, env);
		free(str1);
		free(str2);
		free(str3);
	}
	return (*str);
}


/* Petite fonction main temporaire, d'ici le moment ou 
on trouve ou l'inserer dans notre programme
Je cree un *str qui represente l'equivalent d'un token
qui contient un variable d'environnement ($). Le "free"
final sera "normalement" fait lorsqu'on va free tous les 
tokens. */
int	main(int argc, char **argv, char **env)
{
	char	*str;

	(void)argc;
	(void)argv;
	str = malloc(10 * sizeof(char));
	strcpy(str, "echo'$ARGS'");
	var_expansion(&str, env);
	printf("Le token final est: %s\n", str);
	free(str);
	return (0);
}
