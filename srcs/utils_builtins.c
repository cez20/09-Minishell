/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_builtins.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slavoie <slavoie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 17:35:32 by slavoie           #+#    #+#             */
/*   Updated: 2022/12/05 18:59:48 by slavoie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	remove_quote(t_token *token_list)
{
	char	*temp;
	char	chr;

	chr = simple_or_double(token_list->token);
	if (chr != 32)
	{
		temp = token_list->token;
		token_list->token = ft_strtrim(token_list->token, &chr);
		free(temp);
		token_list->flag_quote = chr;
	}
	else
		token_list->flag_quote = chr;
}

void	quote_remover(t_info *info)
{
	t_command_line	token;
	t_token			*start;
	char	*to_free;
	t_token *temp;
	int		i;

	i = 0;
	while (info->command_lines[i].list_token)
	{
		start = token.list_token;
		token = info->command_lines[i++];
		while (token.list_token)
		{
			to_free = token.list_token->token;
			token.list_token->token = remove_matching_quote(token.list_token->token);
			temp = token.list_token;
			del_empty_node(&temp);
			token.list_token = token.list_token->next;
			free(to_free);
		}
		token.list_token = start;
	}
}

// void	quote_remover(t_command_line **cmd_line)
// {
// 	t_token			*token;
// 	t_token			**tmp;
// 	char			*to_free;
// 	t_command_line	*line;

// 	line = *cmd_line;
// 	token = line->list_token;
// 	while (token)
// 	{
// 		remove_quote(token);
// 		if (token->flag_quote == 32)
// 		{
// 			to_free = token->token;
// 			token->token = ft_strtrim(token->token, " \'\"");
// 			free(to_free);
// 		}
// 		tmp = &token;
// 		token = token->next;
		
// 	}
// }

void	del_empty_node(t_token **token)
{
	t_token	*prev_token;
	t_token	*next_token;

	if (!*token)
		return ;
	prev_token = (*token)->prev;
	next_token = (*token)->next;
	if ((*token)->token && !((*token)->token))
	{
		ft_lstdelone_token(*token, free);
		*token = NULL;
		if (prev_token)
			prev_token->next = next_token;
		if (next_token)
			next_token->prev = prev_token;
	}
}

int	is_n(t_token *node)
{
	int	i;

	i = 1;
	if (node)
	{
		if (ft_strlen(node->token) == 2 && !ft_strncmp(node->token, "-n", 2))
			return (1);
		else if (ft_strlen(node->token) < 2)
			return (0);
		while (node->token[i])
		{
			if (!(node->token[i] == 'n'))
				return (0);
			i++;
		}
		if (!(*node->token) || *node->token == S_QUOTE \
		|| *node->token == D_QUOTE)
			return (0);
		return (1);
	}
	return (0);
}

char	*until_chr(char *str, char c)
{
	int		i;
	char	*line;

	i = 0;
	line = NULL;
	while (str[i] != c)
		i++;
	if (str[i++])
	{
		line = ft_substr(str, 0, i);
		return (line);
	}
	return (line);
}
