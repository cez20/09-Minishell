/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cemenjiv <cemenjiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 09:55:32 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/10/27 14:28:59 by cemenjiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
//Ancien heredoc, juste au cas ou.
// void	create_heredoc(char *delimiter)
// {
// 	char **tab_token;
// 	char *line;
// 	int i;
	
// 	tab_token = NULL;
// 	while (1)
// 	{
// 		line = readline(">");
// 		if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0)
// 		{
// 			free(line);
// 			break;
// 		}
// 		tab_token = tab_join(tab_token, line);
// 		free(line);
// 	}
// 	i = 0;
// 	while (tab_token[i])
// 		printf("%s\n", tab_token[i++]);
// 	free_token(tab_token);
// }


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


// Qu'arrive-til si le fichier est deja cree? On ne veut pas remplacer le fd? 
void	append_document(t_command_line *chunk, char *outfile)
{
	if (chunk->fd_out != 1)
		close(chunk->fd_out);
	chunk->fd_out = open(outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (chunk->fd_in == -1)
	{
		printf("bash: %s: %s\n", outfile, strerror(errno));
	}
	printf("The fd associated with test2.txt est: %d\n", chunk->fd_out);
}


void	open_outfile(t_command_line *chunk, char *token)
{	
	if (chunk->fd_out != 1)
		close(chunk->fd_out);
	chunk->fd_out = open(token, O_TRUNC | O_CREAT | O_RDWR, 0644);
	printf("The fd associated with test2.txt est: %d\n", chunk->fd_out);
}

// Si j'ai plusieurs heredoc comment leur donner des noms differents? 
void	create_heredoc(t_command_line *chunk, char *delimiter)
{
	char	*line;
	
	if (chunk->fd_in != 0)
		close(chunk->fd_in);
	chunk->fd_in = open("heredoc.txt", O_TRUNC | O_CREAT | O_RDWR, 0644);
	while (1)
	{
		line = readline(">");
		if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0)
		{
			free(line);
			break;
		}
		write(chunk->fd_in, line, ft_strlen(line));
		write(chunk->fd_in, "\n", 1);
		free(line);
	}
}

void	open_infile(t_command_line	*cmd_line, t_token	*list_token)
{
	cmd_line->fd_in = open(list_token->next->token, O_RDWR);
	printf("le fd associe avec le infile est %d\n", cmd_line->fd_in);
	if (cmd_line->fd_in == -1 && !(cmd_line->error_infile))
		cmd_line->error_infile = list_token->next->token;
}

void	redirection(t_info	*info)
{
	t_command_line	*chunk;
	t_token			*list;
	int	i;

	i = 0;
	while(i <= info->nb_of_pipe)
	{
		chunk = &info->command_lines[i];
		list = info->command_lines[i].list_token;
		while (list) 
		{
			if ((ft_strncmp(list->token, "<", 2) == 0) && list->next != NULL)
				open_infile(chunk, list);
			else if ((ft_strncmp(list->token, "<<", 2) == 0) && list->next != NULL)
				create_heredoc(chunk, list->next->token);
			else if ((ft_strncmp(list->token, ">", 2) == 0) && list->next != NULL)
				open_outfile(chunk, list->next->token);
			else if ((ft_strncmp(list->token, ">>", 2) == 0) && list->next != NULL)
				append_document(chunk, list->next->token);
			list = list->next;
		}
		i++;
	}
}

// void	redirection(t_info *info)
// {
// 	t_token	*tmp;

// 	tmp = info->list_token;
// 	while (tmp) 
// 	{
// 		if ((ft_strncmp(tmp->token, "<", 2) == 0) && tmp->next != NULL)
// 			info->infile = open_infile(tmp->next->token);
// 		else if ((ft_strncmp(tmp->token, ">", 2) == 0) && tmp->next != NULL)
// 			info->outfile = open_outfile(tmp->next->token); 
// 		else if ((ft_strncmp(tmp->token, "<<", 2) == 0) && tmp->next != NULL) 
// 		 	create_heredoc(tmp->next->token);
// 		else if ((ft_strncmp(tmp->token, ">>", 2) == 0) && tmp->next != NULL)
// 		 	append_document(tmp->next->token);
// 		tmp = tmp->next;
// 	}
// }
