/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slavoie <slavoie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/12 10:10:05 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/11/05 17:58:43 by slavoie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <string.h>
# include "../Libft/libft.h"
# include "../readline/readline.h"
# include "../readline/history.h"
# include <errno.h>
# include <dirent.h>
# include <unistd.h>
# include <signal.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <termios.h>

# define TEXT 32
# define S_QUOTE 39
# define D_QUOTE 34

typedef struct s_token
{
	char			*token;
	int				flag_quote;
	int				space_flag;
	int				redirection;
	struct s_token	*next;
	struct s_token	*prev;
}				t_token;

typedef struct s_info
{
	char					**envp;
	// char	*prompt;
	t_token					*list_token;
	struct s_command_line	*command_lines;
	// int 	flag_quote;
	char					*pwd;
	char					*last_position;
	int						index;
	int						len;
	int						state;
	// int		type_quote;
	int						nb_of_pipe;
	char					**path;
}		t_info;

typedef struct s_command_line
{
	t_token	*list_token;
	char	**cmd_and_args;
	char	*command; // n'est plus necessaire mais aviser Steven 
	char	*args; // N'est plus necessaire mais aviser Steven 
	int		builtin;
	int		fd_in;
	int		fd_out;
	char	*error_infile;
	char	*merge_path_cmd;
}				t_command_line;

//*** MAIN.C ***
char	*search_line(char **tab, char *search);
char	**tab_trunc(char **tab, char *str, int len);
void	token_manager(t_info *info);
int		close_quote_checker(t_info *info, char *str);
int		main(int argc, char **argv, char **envp);

//***BUILTINS.C
void	remove_quote(t_token *token_list);
void	pwd(t_info *info);
void	export(t_info *info);
void	echo(t_info *info);
void	cd(t_info *info);

// *** EXECUTION.C ***
int		create_pipes(t_info *info);
void	execution(t_info *info, t_command_line *line);

//*** FREE.C ***
void	free_double_pointers(char **args);
void	free_struct_command_line(t_info *info);

//*** INIT.C ***
void	init_command_lines(t_command_line *cmd_line, t_info *info);
void	init_info(t_info *info, char **envp);
void	reinit(t_info *info);

//*** PARSING.C ***
void	split_token(char *token, t_info *info);
char	*get_command(t_token *list_token); // On pourra p-e l'enlever
char	*get_args(t_token	*list_token); // On pourra p-e l'enlever
void	fill_command_lines(t_info *info); // On pourra p-e l'enlever
char	*search_another_one(char *str, char c, t_info *info);

//*** SIGNAL.C ***
int		exit_terminal(void); // Function that needs to be worked on
void	sig_handler(int signum);
void	signal_modified(void);
void	disable_echo(void);

//*** VAR_EXPANSION.C ***  VARIABLE avec 5 parametres, c'est trop! 
char	*new_expanded_variable(int i, char *str, char **env);
void	find_expansion(char **str, char *str1, char *str2, char *str3, char **env);
char	*env_variable(char *str, int *i);
void	locate_expansion(char **str, char **env);
void	var_expansion(t_command_line *cmd_line, t_info *info);

//*** REDIRECTION.C ***
void	append_output_redirection(t_command_line *chunk, char *outfile);
void	output_redirection(t_command_line *chunk, char *token);
void	heredoc_redirection(t_command_line *chunk, char *delimiter);
void	input_redirection(t_command_line *cmd_line, t_token *list_token);
int		is_redirection(t_token *list);
void	delete_redirection_tokens(t_token *list_token, t_token **list_addr);
//void	delete_redirection_tokens(t_token **list_addr);
void	delete_tokens(t_token **list);
void	redirection(t_info	*info);

//*** UTILS_1.C ***
void	ft_lstdelone_token(t_token *lst, void (*del)(void *));
int		ft_lstsize_token(t_token *lst);
void	ft_lstclear_token(t_token **lst, void (*del) (void *));
void	lst_print_token(t_token **list);
t_token	*ft_lstnew_token(char *content);
t_token	*ft_lstlast_token(t_token *lst);
void	ft_lstaddback_token(t_token **alst, t_token *new);

//*** UTILS_2.C **
char	simple_or_double(char *token);
int		how_many(t_info *info, char *str, char c);
void	skip_space(t_info *info);
char	**split_path(char **env);
void	print_struct(t_command_line *cmd_line, t_info *info);

//*** UTILS_3.C **
void	is_builtin(t_info *info);
void	fill_cmd(t_info *info);
void	print_double_pointer(char **str);
void	create_exec_argv(t_info	*info);
void	prepare_data_for_execution(t_info *info);

#endif