/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cemenjiv <cemenjiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/12 10:10:05 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/10/28 14:29:06 by cemenjiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../Libft/libft.h"
# include "../readline/readline.h"
# include "../readline/history.h"
# include <errno.h>
# include <dirent.h>
# include <unistd.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <termios.h>

typedef struct s_token
{
	char	*token;
	int		flag_quote;
	int		space_flag;
	int		redirection;


	struct s_token *next;
	struct s_token *prev;
}				t_token;

typedef struct s_info
{
	char			**envp;
	// char	*prompt;
	t_token			*list_token;
	struct s_command_line *command_lines;

	// int 	flag_quote;
	char	*last_position;
	int		index;
	int		nb_of_pipe;
	char	**path;
}		t_info;

typedef struct s_command_line
{
	t_token *list_token;
	// merge command et args
	// char **argv
	char	*command;
	char	*args;
	int		fd_in;
	int		fd_out;
	char	*error_infile;
	char	*merge_path_cmd;
	char 	*paths;
}				t_command_line;

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
void	remove_quote(t_token *token_list);
void	pwd(t_info *info);
void	export(t_info *info);
void	echo(t_info *info);
void	cd(t_info *info);

// *** EXECUTION.C ***
int		create_pipes(t_info *info);
void	execution(t_info *info);

//*** SIGNAL.C ***
void	exit_terminal(); // Function to work on. 
void    sig_handler(int signum);
void    signal_modified();
void 	disable_echo();

//*** VAR_EXPANSION.C ***  VARIABLE avec 5 parametres, c'est trop! 
char	*new_expanded_variable(int i, char *str, char **env);
void	find_expansion(char **str, char *str1, char *str2, char *str3, char **env);
char 	*env_variable(char *str, int *i);
void	locate_expansion(char **str, char **env);
void	var_expansion(t_token *node, char **env);

//*** REDIRECTION.C ***
void	free_token(char **token);
void	append_document(t_command_line *chunk, char *outfile);
void	open_outfile(t_command_line *chunk, char *token);
void	create_heredoc(t_command_line *chunk, char *delimiter);
void	open_infile(t_command_line	*command_line, t_token	*token);
void	redirection(t_info	*info);
void	delete_redirection_tokens(t_token *list_token, t_token **lis);
//void	delete_redirection_tokens(t_token *list_token);
void	delete_tokens(t_token **list);
int		is_redirection_operator(t_token	*list);

//*** UTILS.C ***
void	del(void *token);
void	ft_lstaddback_token(t_token **alst, t_token *new);
t_token	*ft_lstnew_token(char *content);
void 	lst_print_token(t_token **list);
void	ft_lstclear_token(t_token **lst, void (*del) (void *));
t_token	*ft_lstlast_token(t_token *lst);


//*** UTILS_2.C **
int		how_many(char *str, char c);
void	fill_command_lines(t_info *info);
void 	skip_space(t_info *info);
char	*search_another_one(char *str, char c, t_info *info);
char	**split_path(char **env);
void	init_struct(t_command_line *cmd_line, t_info *info);
void	print_struct(t_command_line *cmd_line, t_info *info);


#endif