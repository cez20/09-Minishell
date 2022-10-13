/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cemenjiv <cemenjiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 13:43:50 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/10/12 16:38:55 by cemenjiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

