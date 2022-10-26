/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cemenjiv <cemenjiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 09:55:32 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/10/25 23:13:35 by cemenjiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void free_token(char **token)
{
	int i;

	i = 0;
	while (token[i])
	{
		free(token[i]);
		i++;
	}
	free(token);
}

//See about opendir(), readdir(), closedir() functions
//If an outfile is already open, a fd has already been associated to it
//Another open() function will open another fd and won't cause any trouble. 
void	append_document(char *outfile)
{
	char *str;
	int file;
 
	file = open(outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (file == -1)
	{
		//First way of doing it: 
		printf("bash: %s: %s\n", outfile, strerror(errno));
		//Second way of doing message error below:
		str = ft_strjoin("bash: ", outfile);
		perror(str);
		free(str);	
	}
	write(file, "I am tired!\n", 12);	//This will write I am tired, at the end of file. 
}

//Verify why there are so many still reachable when doing this type of heredoc 
//Use rl_clear_history which is different than clear_history to remove still reachable??
//Print content only if followed cat command 
//If a pipe directly after heredoc (ex: << Cesar | wc ), input of heredoc is denied. Following command is executed with no correct input
//Normally, bash does not create a file that is visible to user. Therefore, might be best to  create a double pointer (tab_join function)
void	create_heredoc(char *delimiter)
{
	char **tab_token;
	char *line;
	int i;
	
	tab_token = NULL;
	while (1)
	{
		line = readline(">");
		if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0)
		{
			free(line);
			break;
		}
		tab_token = tab_join(tab_token, line);
		free(line);
	}
	i = 0;
	while (tab_token[i])
		printf("%s\n", tab_token[i++]);
	free_token(tab_token);
}

int	open_outfile(char *token)
{
	int outfile;
	
	outfile = open(token, O_TRUNC | O_CREAT | O_RDWR, 0644);
	return (outfile);
}

int	open_infile(char *token)
{
	char *str;
	int infile;
	
	str = NULL;
	infile = open(token, O_RDWR);
	printf("Je suis dans le infile\n");
	if (infile == -1) // Le 1er argument de open() est le path ou se trouve le fichier. Il faudrait
		printf("bash: %s: %s\n", token, strerror(errno));
	return (infile);
}

// void	redirection(t_command_line *command_lines)
// {
// 	t_command_line	*chunk;
// 	int i;

// 	i = 0;
// 	chunk = command_lines;
// 	while (chunk[i].list_token) 
// 	{
// 			if ((ft_strncmp(chunk[i].list_token->token, "<", 2) == 0) && chunk[i].list_token->next->token != NULL)
// 				printf("Je suis en business mon chum!\n");
// 					info->infile = open_infile(tmp->next->token);
// 				else if ((ft_strncmp(tmp->token, ">", 2) == 0) && tmp->next != NULL)
// 					info->outfile = open_outfile(tmp->next->token); 
// 				else if ((ft_strncmp(tmp->token, "<<", 2) == 0) && tmp->next != NULL) 
// 				 	create_heredoc(tmp->next->token);
// 				else if ((ft_strncmp(tmp->token, ">>", 2) == 0) && tmp->next != NULL)
// 				 	append_document(tmp->next->token);
// 			printf("%s\n", chunk[i].list_token->token);
// 			chunk[i].list_token = chunk[i].list_token->next;
// 	}
// }

void	redirection(t_info	*info)
{
	t_command_line	*chunk;
	int i;

	i = 0;
	chunk = info->command_lines;
	while(i <= info->nb_of_pipe)
	{
		while (chunk[i].list_token) 
		{
			if ((ft_strncmp(chunk[i].list_token->token, "<", 2) == 0) && chunk[i].list_token->next != NULL)
				printf("Je suis en business mon chum!\n");
					//info->infile = open_infile(tmp->next->token);
				// else if ((ft_strncmp(tmp->token, ">", 2) == 0) && tmp->next != NULL)
				// 	info->outfile = open_outfile(tmp->next->token); 
				// else if ((ft_strncmp(tmp->token, "<<", 2) == 0) && tmp->next != NULL) 
				//  	create_heredoc(tmp->next->token);
				// else if ((ft_strncmp(tmp->token, ">>", 2) == 0) && tmp->next != NULL)
				//  	append_document(tmp->next->token);
			printf("%s\n", chunk[i].list_token->token);
			chunk[i].list_token = chunk[i].list_token->next;
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
