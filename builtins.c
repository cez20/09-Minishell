/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cemenjiv <cemenjiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/12 09:45:30 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/10/12 09:45:45 by cemenjiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_quote(char **str, t_info	*info)
{
	char	*temp;
	char	chr;

	chr = simple_or_double(*str);

	// printf("%c\n", chr);
	// printf("%c\n", str[0][ft_strlen(*str) - 2]);

	if (str[0][ft_strlen(*str) - 1] == chr && chr != 32)
	{
		temp = *str;
		*str = ft_substr(*str, 1, ft_strlen(*str) - 2);
		free(temp);
		info->flag_quote = 1;
	}
	else
		info->flag_quote = 0;

	
}

void	pwd(t_info *info)
{
	char *pwd;

	pwd = search_line(info->envp, "PWD=");
	pwd = ft_strchr(pwd, '=');
	printf("%s\n", ++pwd);
}

/*
		hypothèse pour gérer "" et '' coller 

		flag dans la struct si oui ou non il y avait des quotes donc une struct token

		flag dans la fonction

		ne pas enlever les quotes, mais simplement ne pas les afficher 

*/


void	echo(t_info *info)
{
	int i;
	t_token	*token_list;

	token_list = info->list_token;
	info->flag_quote = 0;

	if (!ft_strncmp(token_list->next->token, "-n", 2))
		i = 2;
	else
		i = 1;

	while(token_list->token)
	{
		// printf("%c\n", info->token[i][0]);
		if (info->flag_quote == 1 && *token_list->token == simple_or_double(info->token[i])) 
		{
			// remove_quote(&info->token[i], info);
			printf("%s", info->token[i++]);
		}
		else
		{
			// remove_quote(&info->token[i], info);
			printf(" %s", info->token[i++]);
		}
		token_list = token_list->next;
	}
	if (ft_strncmp(info->list_token->next->token, "-n", 2))
		printf("\n");
}