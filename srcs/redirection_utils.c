/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slavoie <slavoie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/19 20:43:08 by slavoie           #+#    #+#             */
/*   Updated: 2022/11/19 20:44:36 by slavoie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	output_redirection(t_command_line *chunk, char *token)
{	
	if (chunk->fd_out != 1)
		close(chunk->fd_out);
	chunk->fd_out = open(token, O_TRUNC | O_CREAT | O_RDWR, 0644);
}

void	input_redirection(t_command_line *cmd_line, t_token *list_token)
{
	cmd_line->fd_in = open(list_token->next->token, O_RDWR);
	if (cmd_line->fd_in == -1 && !(cmd_line->error_infile))
	{
		cmd_line->error_infile = ft_strdup(list_token->next->token);
		cmd_line->fd_in = 0;
	}
}
