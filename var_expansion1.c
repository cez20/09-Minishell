/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expansion1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cemenjiv <cemenjiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 12:27:16 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/10/10 13:38:58 by cemenjiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Libft/libft.h"
#include <string.h>


int	ft_isalpha1(int c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
		return (1);
	else
		return (0);
}


char	*new_expanded_variable(int i, char *str, char **env)
{
	int	j;

	j = 0;
	while (env[i][j] == str[j] && str[j])
		j++;
	str = &env[i][j];
	return (str);
}

void	find_expansion(char **str, char *str1, char *str2, char *str3, char **env)
{
	char *string;
	int	i;

	i = 0;
	while (env[i] && ft_strnstr(env[i], str2, ft_strlen(str2)) == 0)
		i++;
	if (env[i])
	{
		free(*str);
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

char *new_string(char *str, int *i)
{
	char *str1;
	int j;

	j = *i;
	while (ft_isalpha1(str[*i]) != 0)
		(*i)++;
	str1 = ft_substr(str, j, *i - j);
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
		find_expansion(str, str1, str3, str4, env);
		free(str1);
		free(str2);
		free(str3);
		free(str4);
	}
	return (*str);
}

int	main(int argc, char **argv, char **env)
{
	char	*str;

	(void)argc;
	(void)argv;
	str = malloc(12 * sizeof(char));
	strcpy(str, "echo'$ARGS'");
	var_expansion(&str, env);
	printf("Le token final est: %s\n", str);
	free(str);
	return (0);
}
