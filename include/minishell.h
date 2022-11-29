/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slavoie <slavoie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/12 10:10:05 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/11/29 16:14:57 by slavoie          ###   ########.fr       */
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
	t_token					*list_token;
	struct s_command_line	*command_lines;
	char					*pwd;
	char					*last_position;
	int						index;
	int						len;
	int						state;
	int						nb_token;
	int						nb_of_pipe;
	int						initial_stdin;
	int						initial_stdout;
	int						exit_code;
	int						read_pipe;
	char					**paths;
	int						herestring;
}		t_info;

typedef struct s_command_line
{
	t_token	*list_token;
	char	*command;
	char	*args;
	int		builtin;
	int		fd_in;
	int		fd_out;
	int		chevron;
	char	*error_infile;
	char	*error_outfile;
	char	*path;
	char	**argv;
}				t_command_line;

int	g_fd_in;
int		close_quote_checker(t_info *info, char *str);
char	*search_line(char **tab, char *search);
void	remove_quote(t_token *token_list);
//*** MAIN.C ***
char	**tab_trunc(char **tab, char *str, int len);
void	token_manager(t_info *info);
char	*take_input(char *prompt);
int		main(int argc, char **argv, char **envp);


//*** ARGS_CHECKER.C ***
int		check_arg_export(char *arg, t_info *info);
int		check_arg_unset(char *arg, t_info *info);
char	*check_chevron(t_info *info);

//***BUILTINS.C
void	pwd(t_info *info);
void	echo(t_info *info);
void	cd(t_info *info);
void	export(t_info *info);
void	unset(t_info *info);

//*** CLOSE_FDS.C ***
void	close_unused_fds_heredoc(t_command_line *cmd_line, int current_index);
void	close_current_fds(t_command_line *cmd_line, t_info *info);
void	close_unused_fds(t_command_line *cmd_line, t_info *info);

//*** ERROR.C ***
void	check_if_error(t_command_line cmd_line, t_info *info);
void	syntax_error(void);
void	no_file(char *str);
void	command_not_found(char *str);

// *** EXECUTION.C ***
void	close_current_fds(t_command_line *cmd_line, t_info *info);
void	close_unused_fds(t_command_line *cmd_line, t_info *info);
void	last_child_process(t_command_line *cmd_line, t_info *info, pid_t *pid);
void	child_process(t_command_line *cmd_line, t_info *info, pid_t *pid);
void	multiple_commands_or_builtins(t_command_line *cmd_line, t_info *info);
void	execution(t_info *info, t_command_line *line);

//*** FREE.C ***
void	free_double_pointers(char **args);
void	free_struct_command_line(t_info *info);
void	free_tab(char **tab);

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
void	signal_inside_heredoc(int signum);
void	signal_outside_heredoc(int signum);
int		get_exit_code(int exit_code);
int		exit_terminal(t_info *info, int flag, int exit_code);
void	signal_child(int signum);
void	signal_parent(int signum);
void	enable_signals(void);
void	disable_signals(void);

//*** REDIRECTION.C ***
void	append_output_redirection(t_command_line *chunk, char *outfile);
void	close_unused(t_command_line *cmd_line, int current_index);
void	delimiter_finder(t_info *info, char *delimiter, int fd[]);
void	output_redirection(t_command_line *chunk, char *token);
void	heredoc_redirection(t_command_line *cmd_line, \
char *delimiter, t_info *info, int i);
void	search_for_redirection(t_info	*info);

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

//*** PREPARE_EXEC.C **
void	is_builtin(t_info *info);
void	find_path_of_command(t_command_line *cmd_line, char *path);
void	find_execve_path(t_info *info, t_command_line *cmd_line);
void	create_execve_argv(t_info *info, t_command_line *cmd_line);
void	prepare_data_for_execution(t_info *info);

//*** UTILS_BUILTINS.C ***
void	remove_quote(t_token *token_list);
void	quote_remover(t_command_line **cmd_line);
void	del_empty_node(t_token **token);
int		is_n(t_token *node);
char	*until_chr(char *str, char c);

//*** UTILS_EXECUTION.C ***
void	exec_one_command(t_command_line cmd_line, t_info *info);
void	one_command_or_builtin(t_command_line *cmd_line, t_info *info);
void	put_back_default_std(t_info *info);
void	do_redirection(t_command_line cmd_line, t_info *info);

///*** UTILS_PRINT.C ***
void	print_struct(t_command_line *cmd_line, t_info *info);
void	print_double_pointer(char **str);

//*** UTILS_REDIRECTION.C ***
void	delete_tokens(t_token **list);
int		is_redirection(t_token *list);
void	delete_redirection_tokens(t_token *list_token, t_token **list_addr);

//*** UTILS_EXPANSION.C ***
void	chop_chop(char **str, char **tab, char **env, int i);
void	find_expansion(char **str, char **tab, char **env);
void	if_exit_code(char **str, t_info *info);

//*** VAR_EXPANSION.C ***  VARIABLE avec 5 parametres, c'est trop! 
char	*new_expanded_variable(int i, char *str, char **env);
void	find_expansion(char **str, char **tab, char **env);
char	*env_variable(char *str, int *i);
void	locate_expansion(char **str, char **env, t_info *info);
void	var_expansion(t_command_line *cmd_line, t_info *info);

// void	garbage_collector(t_info *info);

void	trim_space(t_info *info, char *set);
void	enable_signals_minishell(void);
void	free_info(t_info *info);
char	*get_command(t_token *list_token);
void	export_routine(t_info *info, char *str, int i);
void	export_no_args(t_info *info, char *str);
void	echo_routine(t_token *token_list);
void	input_redirection(t_command_line *cmd_line, char *infile);
void	remove_inside_quote(t_info *info);
void	close_fds(int *fd);
void	print_error_cd(char *str);

#endif