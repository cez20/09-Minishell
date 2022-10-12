/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cemenjiv <cemenjiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 09:55:32 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/10/12 15:14:05 by cemenjiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	write(file, "I am tired!\n", 11);	//This will write I am tired, at the end of file. 
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

int	open_infile(char *token)
{
	char *str; 
	int infile;
	
	str = NULL;
	infile = open(token, O_RDWR);
	printf("Je suis dans le infile\n");
	if (infile == -1) // Le 1er argument de open() est le path ou se trouve le fichier. Il faudrait
	{
		//First way of doing it: 
		printf("bash: %s: %s\n", token, strerror(errno));
		//Second way of doing message error below:
		str = ft_strjoin("bash: ", token);
		perror(str);
		free(str);	
	}
	return (infile);
}

void redirection(char **token, int *infile, int *outfile)
{
	int i;

	i = 0;
	while (token[i]) 
	{
		if ((ft_strncmp(token[i], "<", 2) == 0) && token[i + 1] != 0)
			*infile = open_infile(token[i + 1]);
		else if ((ft_strncmp(token[i], ">", 2) == 0) && token[i + 1] != 0)
			*outfile = open(token[i + 1], O_TRUNC | O_CREAT | O_RDWR, 0644);
		else if ((ft_strncmp(token[i], "<<", 2) == 0) && token[i + 1] != 0) 
			create_heredoc(token[i + 1]);
		else if ((ft_strncmp(token[i], ">>", 2) == 0) && token[i + 1] != 0)
			append_document(token[i + 1]);
		i++;
	}
}

// int main ()
// {
// 	char **token;
// 	char *str = "< ../test.txt | wc -l > test1.txt << Cesar >> test2.txt";
// 	int fd_infile;
// 	int fd_outfile;

// 	token = ft_split(str, ' ');
// 	redirection(token, &fd_infile, &fd_outfile);
// 	free_token(token);
// 	close(fd_infile);
// 	close(fd_outfile);
// 	return (0);
// }