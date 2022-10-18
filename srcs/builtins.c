/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slavoie <slavoie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/12 09:45:30 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/10/17 15:47:34 by slavoie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

void	pwd()
{
	char pwd[4096];

	// pwd = search_line(info->envp, "PWD=");
	// pwd = ft_strchr(pwd, '=');
	getcwd(pwd, 4096);
	printf("%s\n", pwd);
}

/*
		hypothèse pour gérer "" et '' coller 

		flag dans la struct si oui ou non il y avait des quotes donc une struct token

		flag dans la fonction

		ne pas enlever les quotes, mais simplement ne pas les afficher 

*/

// int	is_all_n(char *token)
// {
// 	while (*token)
// 	{
// 		token++;
// 		printf("*token = %c\n", *token);
// 		if (*token == '\0')
// 			return (1);
// 		else if (*token != 'n')
// 			return (0);
// 	}
// 	return (1);
// }



void	echo(t_info *info)
{
	t_token	*token_list;
	int	i;

	i = 0;

	token_list = info->list_token->next;
	if (token_list)
	{
		if (!ft_strncmp(token_list->token, "-n", 2))
			token_list = token_list->next;
	}

	// printf("allo\n");
	while(token_list)
	{
		// printf("%c\n", info->token[i][0]);
		// printf("\nflag = %d", token_list->space_flag);
		if (token_list->prev->space_flag == 1 && i > 0) 
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
		i++;
		token_list = token_list->next;
	}
	if (info->list_token->next)
	{
		if (ft_strncmp(info->list_token->next->token, "-n", 2))
			printf("\n");
	}
	else
		printf("\n");
		
}

void	cd(t_info *info)
{
	char *new_path;

	if (info->list_token->next)
		new_path = info->list_token->next->token;
	else
		new_path = getenv("HOME");
	if (chdir(new_path) != 0 && ((ft_strncmp(new_path, ".", 1) && ft_strncmp(new_path, "..", 2)) || !ft_strncmp(new_path, "...", 3)))
		printf("cd: %s: No such file or directory\n", new_path);
	chdir(new_path);
}