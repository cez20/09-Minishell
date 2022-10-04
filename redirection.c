/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cemenjiv <cemenjiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 09:55:32 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/10/04 13:08:28 by cemenjiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include <string.h>
#include <errno.h> // I am authorized to use this ??

//Elements a travailler:
// 1 - (FAIT, gerer par la tokenisation)NE PAS oublier, que premier argument de open est UN PATH (donc ../test.txt ou test.txt ../../test.txt sont des paths)
// 2 - (FAIT) Il faut que je garde en memoire, le FD qui est associe a l'ouverture du infile pour pouvoir le fermer a la fin du programme
// 3 - (A PORTER ATTENTION) Les output file sont toujours cree peu importe le cas, mais si la command anterieur FAIL, le message d'erreur
//     de la commande apparaitra sur le STDOUT. DONC, le message NE sera PAS redirige vers le fichier (ex: echo Cesar | wc -p > test.txt)
//     wc -p renverra un message d'erreur  car cette commande n'existe pas. Fichier test.txt va etre cree mais rien contenir.  
// 4 - (FAIT, AUCUN PROBLEME)Est-ce que le fait d'ouvrir le output avant meme l'execution des commandes causera problemes. Je ne crois pas. Fichier vide qui attendra contenu. 
// 5 - (FAIT, AUCUN LEAKS, AUCUN FD OUVERT)Verifier pour des LEAKS.
// 6 - Comment le input devient-il le input de la commande qui suit? Faire un micro example

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


int	open_infile(char *token)
{
	char *string; 
	int infile;
	
	string = NULL;
	infile = open(token, O_RDWR);
	if (infile == -1) // Le 1er argument de open() est le path ou se trouve le fichier. Il faudrait
	{
		//First way of doing it: 
		printf("bash: %s: %s\n", token, strerror(errno));
		//Second way of doing message error below:
		string = ft_strjoin("bash: ", token);
		perror(string);
		free(string);	
	}
	return (infile);
}

void indirection(char **token, int *infile, int *outfile)
{
	int i;

	i = 0;
	while (token[i]) // Tant que la valeur du token[i] est != 0
	{
		if (*token[i] == '<' && *token[i + 1] != '\0')
			*infile = open_infile(token[i + 1]);
		if (*token[i] == '>' && *token[i + 1] != '\0')
			*outfile = open(token[i + 1], O_TRUNC | O_CREAT | O_RDWR, 0644);
		i++;
	}
}

int main ()
{
	char **token;
	char *str = "< ../test.txt | wc -l > test1.txt";
	int fd_infile;
	int fd_outfile;

	token = ft_split(str, ' ');
	indirection(token, &fd_infile, &fd_outfile);
	free_token(token);
	close(fd_infile);
	close(fd_outfile);
	return (0);
}