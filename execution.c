/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cemenjiv <cemenjiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 13:43:50 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/10/12 10:08:02 by cemenjiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "Libft/libft.h"

// void free_token(char **token)
// {
// 	int i;

// 	i = 0;
// 	while (token[i])
// 	{
// 		free(token[i]);
// 		i++;
// 	}
// 	free(token);
// }

// int	ft_isalpha1(int c)
// {
// 	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
// 		return (1);
// 	else
// 		return (0);
// }

// char	*new_expanded_variable(int i, char *str, char **env)
// {
// 	int	j;

// 	j = 0;
// 	while (env[i][j] == str[j] && str[j])
// 		j++;
// 	str = &env[i][j];
// 	return (str);
// }

// void	find_expansion(char **str, char *str1, char *str2, char *str3, char **env)
// {
// 	char *string;
// 	int	i;

// 	i = 0;
// 	while (env[i] && ft_strnstr(env[i], str2, ft_strlen(str2)) == 0)
// 		i++;
// 	if (env[i])
// 	{
// 		free(*str);
// 		if (str1)
// 		{
// 			string = ft_strjoin(str1, new_expanded_variable(i, str2, env));
// 			*str = ft_strjoin(string, str3);
// 			free(string);
// 		}
// 		else
// 		{
// 			string = ft_strdup(new_expanded_variable(i, str2, env));
// 			*str = ft_strjoin(string, str3);
// 			free(string);
// 		}
// 	}
// }

// char *new_string(char *str, int *i)
// {
// 	char *str1;
// 	int j;

// 	j = *i;
// 	while (ft_isalpha1(str[*i]) == 1)
// 		(*i)++;
// 	str1 = ft_substr(str, j, (*i - j));
// 	return (str1);
// }

// char	*var_expansion(char **str, char **env)
// {
// 	char	*str1;
// 	char	*str2;
// 	char	*str3;
// 	char	*str4;
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
// 		str2 = new_string(*str, &i);
// 		str3 = ft_strjoin(str2, "=");
// 		str4 = ft_strdup(*str + i);
// 		find_expansion(str, str1, str3, str4, env);
// 		free(str1);
// 		free(str2);
// 		free(str3);
// 		free(str4);
// 	}
// 	return (*str);
// }

void	remove_extra_quote(char **token, char quote)
{
	char *tmp_token;

	tmp_token = ft_strdup(*token);
	free(*token); 
	if (quote == 34)
		*token = ft_strtrim(tmp_token, "\""); 
	else if (quote == 39)
		*token = ft_strtrim(tmp_token, "\'");
	free(tmp_token);
	tmp_token = NULL;
	
}

// int main (int argc, char **argv, char **env)
// {
// 	(void)argc;
// 	(void)argv;
// 	char **token;
// 	char *str = "< $ARGS cat | \"echo'$ALLO'\" | 'wc' -l > test2.txt";
// 	int infile;
// 	int outfile; 
// 	int i;

// 	// Cette partie fait l'expansion des variables d'environnment
// 	// Et enleve aussi tous les extra quotes, une fois l'expansion faite 
// 	i = 0;
// 	token = ft_split(str, ' ');
// 	while (token[i])
// 	{
// 		if (ft_strchr(token[i], '$') && token[i][0] != 39)
// 			var_expansion(&token[i], env);
// 		if (token[i][0] == 34 || token[i][0] == 39 )
// 			remove_extra_quote(&token[i], token[i][0]);
	
// 		i++;
// 	}

// 	//Cette section reimprime les tokens suite apres avoir passe les 2 fonctions. 
// 	i = 0;
// 	while(token[i])
// 		printf("%s\n", token[i++]);

// 	// Cette section va faire les redirections
// 	redirection(token, &infile, &outfile);
	
// 	free_token(token);
// }

