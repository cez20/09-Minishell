/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cemenjiv <cemenjiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/12 10:10:05 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/10/12 14:05:44 by cemenjiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include "Libft/libft.h"
#include "readline/readline.h"
#include "readline/history.h"
#include <errno.h>
#include <dirent.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <termios.h>

typedef struct s_token
{
	char	*token;
	int		flag_quote;
	int		space_flag;
	struct	s_token *next;
}				t_token;

typedef struct s_info
{
	char 	**envp;
	char 	**token;
	t_token	*list_token;
	int 	flag_quote;
	char 	*last_position;
	int 	infile;
	int 	outfile;
	int 	nb_of_pipe;
}		t_info;

//*** MAIN.C ***
void	print_tab(char **tab);
char	*get_path(char *command, char **envp);
void	command_exeggutor(char *argv, char **envp);
char	*search_line(char **tab, char *search);
void 	token_manager(t_info *info);
char 	simple_or_double(char *token);
void	split_token(char *token, t_info *info);
int 	main(int argc, char **argv, char **envp);

//***BUILTINS.C
void	remove_quote(char **str, t_info	*info);
void	pwd(t_info *info);
void	echo(t_info *info);

// *** EXECUTION.C *** 
void	remove_extra_quote(char **token, char quote);

//*** SIGNAL.C ***
void	exit_terminal(); // Function to work on. 
void    sig_handler(int signum);
void    signal_modified();
void 	disable_echo();

//*** VAR_EXPANSION.C ***  VARIABLE avec 5 parametres, c'est trop! 
int		ft_isalpha1(int c);
char	*new_expanded_variable(int i, char *str, char **env);
void	find_expansion(char **str, char *str1, char *str2, char *str3, char **env);
char 	*env_variable(char *str, int *i);
char	*var_expansion(char **str, char **env);

//*** REDIRECTION.C ***
void	free_token(char **token);
void	append_document(char *outfile);
void	create_heredoc(char *delimiter);
int		open_infile(char *token);
void 	redirection(char **token, int *infile, int *outfile);

//*** UTILS.C ***
void	ft_lstaddback_token(t_token **alst, t_token *new);
t_token	*ft_lstnew_token(char *content);
void 	lst_print_token(t_token **list);
void	ft_lstclear_token(t_token **lst, void (*del) (void *));

#endif