/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cemenjiv <cemenjiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 09:55:32 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/11/22 12:26:29 by cemenjiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// Qu'arrive-til si le fichier est deja cree? On ne veut pas remplacer le fd? 
void	append_output_redirection(t_command_line *chunk, char *outfile)
{
	if (chunk->fd_out != 1)
		close(chunk->fd_out);
	chunk->fd_out = open(outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (chunk->fd_in == -1)
	{
		printf("bash: %s: %s\n", outfile, strerror(errno));
	}
}

void	output_redirection(t_command_line *chunk, char *token)
{	
	if (chunk->fd_out != 1)
		close(chunk->fd_out);
	chunk->fd_out = open(token, O_TRUNC | O_CREAT | O_RDWR, 0644);
}

void	delimiter_finder(char *line, char *delimiter, int fd[])
{
	close(fd[0]);
	fd_in = fd[1];
	line = readline(">");
	while(1)
	{
		if ((ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0) && \
		ft_strlen(delimiter) == ft_strlen(line))
		{
			close(fd[1]);
			free(line);
			exit (EXIT_SUCCESS);
		}
		else if (!line)
		{
			close(fd[1]);
			exit(EXIT_SUCCESS);
		}
		write(fd[1], line, ft_strlen(line));
		write(fd[1], "\n", 1);
		free(line);
		line = readline(">");
	}
}

// Si j'ai plusieurs heredoc comment leur donner des noms differents? 
void	heredoc_redirection(t_command_line *cmd_line, char *delimiter)
{
	char	*line;
	int		fd[2];
	pid_t	pid;

	if (pipe(fd) == -1)
		return ;
	pid = fork();
	line = NULL;
	if (pid == 0)
	{
		signal(SIGINT, &signal_inside_heredoc);
		delimiter_finder(line, delimiter, fd);
	}
	signal(SIGINT, &signal_heredoc);
	close (fd[1]);
	cmd_line->fd_in = fd[0];
	waitpid(pid, NULL, 0);
}

// void	heredoc_redirection(t_command_line *cmd_line, char *delimiter, t_info *info)
// {
// 	char	*line;
// 	int		fd[2];
// 	pid_t	pid;
// 	int		status;
	
// 	if (pipe(fd) == -1)
// 		return ;
// 	pid = fork();
// 	cmd_line->fd_in = open("heredoc.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);
// 	signal(SIGINT, &signal_heredoc);
// 	if (pid == 0)
// 	{
// 		signal(SIGINT, &signal_heredoc);
// 		close (fd[1]);
// 		cmd_line->fd_in = fd[0];
// 		waitpid(pid, NULL, 0);
// 		line = readline(">");
// 		while(1)
// 		{
// 			if ((ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0) && \
// 			ft_strlen(delimiter) == ft_strlen(line))
// 			{
// 				close (cmd_line->fd_in);
// 				free(line);
// 				exit (EXIT_SUCCESS);
// 			}
// 			else if (!line)
// 			{
// 				close (cmd_line->fd_in);
// 				exit(EXIT_SUCCESS);
// 			}
// 			write(cmd_line->fd_in, line, ft_strlen(line));
// 			write(cmd_line->fd_in, "\n", 1);
// 			free(line);
// 			line = readline(">");
// 		}
// 	}
// 	waitpid(pid, &status, 0);
// 	info->exit_code = get_exit_code(status);
// 	close(cmd_line->fd_in);
// 	cmd_line->fd_in = open("heredoc.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);
// }

void	input_redirection(t_command_line *cmd_line, t_token *list_token)
{
	cmd_line->fd_in = open(list_token->next->token, O_RDWR);
	if (cmd_line->fd_in == -1 && !(cmd_line->error_infile))
	{
		cmd_line->error_infile = ft_strdup(list_token->next->token);
		cmd_line->fd_in = 0;
	}
}

void	search_for_redirection(t_info *info)
{
	t_command_line	*chunk;
	t_token			*list;
	int				i;

	i = 0;
	while (i <= info->nb_of_pipe)
	{
		chunk = &info->command_lines[i];
		list = info->command_lines[i].list_token;
		while (list)
		{
			if ((ft_strncmp(list->token, "<", 2) == 0) && list->next)
				input_redirection(chunk, list);
			else if ((ft_strncmp(list->token, "<<", 3) == 0) && list->next)
				heredoc_redirection(chunk, list->next->token);
				//heredoc_redirection(chunk, list->next->token, info);
			else if ((ft_strncmp(list->token, ">", 2) == 0) && list->next)
				output_redirection(chunk, list->next->token);
			else if ((ft_strncmp(list->token, ">>", 2) == 0) && list->next)
				append_output_redirection(chunk, list->next->token);
			list = list->next;
		}
		delete_redirection_tokens(info->command_lines[i]. \
		list_token, &info->command_lines[i].list_token);
		i++;
	}
}
