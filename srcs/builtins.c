/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cemenjiv <cemenjiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/12 09:45:30 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/10/13 22:58:01 by cemenjiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/minishell.h"

void	remove_quote(t_token *token_list)
{
	char	*temp;
	char	chr;

	chr = simple_or_double(token_list->token);

	// printf("%c\n", chr);
	// printf("%c\n", str[0][ft_strlen(*str) - 2]);

	if (token_list->token[ft_strlen(token_list->token) - 1] == chr && chr != 32)
	{
		temp = token_list->token;
		token_list->token = ft_substr(token_list->token, 1, ft_strlen(token_list->token) - 2);
		free(temp);
		token_list->flag_quote = 1;
	}
	else
		token_list->flag_quote = 0;

	
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
	t_token	*token_list;

	token_list = info->list_token->next;
	info->flag_quote = 0;
	if (!ft_strncmp(token_list->token, "-n", 2))
		token_list = token_list->next;

	// printf("allo\n");
	while(token_list)
	{
		// printf("%c\n", info->token[i][0]);
		if (token_list->prev->space_flag == 1) 
		{
			remove_quote(token_list);
			printf(" %s", token_list->token);
		}
		else
		{
			remove_quote(token_list);
			printf("%s", token_list->token);
			// printf("%s", token_list->token);
		}
		// printf("%s\n", token_list->token);
		token_list = token_list->next;
	}
	if (ft_strncmp(info->list_token->next->token, "-n", 2))
		printf("\n");
}