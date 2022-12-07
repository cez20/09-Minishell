/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slavoie <slavoie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/12 10:10:05 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/12/06 15:36:14 by slavoie          ###   ########.fr       */
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
	int						err_chevron;
	int						err_happen;
	char					**paths;
	int						heredoc;
}		t_info;

typedef struct s_command_line
{
	t_token	*list_token;
	char	*command;
	char	*args;
	int		builtin;
	int		fd_in;
	int		fd_out;
	int		file_after_chevron;
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
int		arg_exit(t_info *info);
void	token_manager(t_info *info);
char	*take_input(char *prompt);
void	routine(t_info *info, char *line);
int		main(int argc, char **argv, char **envp);

//*** ARGS_CHECKER.C ***
int		check_arg_export(char *arg, t_info *info);
int		check_arg_unset(char *arg, t_info *info);
char	*input_chevron(t_info *info, int i);
char	*output_chevron(t_info *info, int i);
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
void	close_fds(int *fd);

//*** ERROR.C ***
void	check_if_error(t_command_line cmd_line, t_info *info);
void	syntax_error(t_info *info) ;
void	no_file(t_info *info, char *str);
void	command_not_found(t_info *info, char *str);
void	not_executable(t_info *info, char *str);

// *** EXECUTION.C ***
void	do_execution(t_command_line cmd_line, t_info *info);
void	last_child_process(t_command_line *cmd_line, t_info *info, pid_t *pid);
void	child_process(t_command_line *cmd_line, t_info *info, pid_t *pid);
void	multiple_commands_or_builtins(t_command_line *cmd_line, t_info *info);
void	execution(t_info *info, t_command_line *cmd_line);

// *** EXIT.C ***
int		get_exit_code(int status);
void	if_exit_code(char **str, t_info *info);
int		parse_error(t_info *info);
int		exit_terminal(t_info *info, int flag, int exit_code);

// *** EXPORT_UTILS.C ***
void	export_routine(t_info *info, char *str, int i);
void	export_no_args(t_info *info, char *str);
void	echo_routine(t_token *token_list);
void	little_main_routine(char *line, t_info *info);
void	routine(t_info *info, char *line);

//*** FREE.C ***
void	free_struct_command_line(t_info *info);
void	free_info(t_info *info);
void	free_tab(char **tab);
void	free_structs_and_exit(t_info *info, int exit_number);

//*** INIT.C ***
void	init_command_lines(t_command_line *cmd_line, t_info *info);
void	init_info(t_info *info, char **envp);
void	reinit(t_info *info);

//*** PARSING_UTILS.C ***
char	*check_double_quote(char *str, t_info *info);
char	*check_simple_quote(char *str, t_info *info);
int		close_quote_checker(t_info *info, char *str);
void	ft_str_tolower(char *str);
char	*get_command(t_token *list_token);

//*** PARSING.C ***

void	fill_command_lines(t_info *info);
char	*set_start(t_info *info, char c, char **start, char *str);
char	*search_another_one(char *str, char c, t_info *info);

//*** PREPARE_EXEC.C  ***
void	is_builtin(t_info *info);
void	find_execve_path(t_info *info, t_command_line *cmd_line);
void	find_path_of_command(t_command_line *cmd_line, char *path);
void	create_execve_argv(t_info	*info, t_command_line *cmd_line);
void	prepare_data_for_execution(t_info *info);

//*** REDIRECTION.C ***
void	append_output_redirection(t_command_line *cmd_line, char *outfile);
void	output_redirection(t_command_line *cmd_line, char *outfile);
void	delimiter_finder(t_info *info, char *delimiter, int fd[]);
void	heredoc_redirection(t_command_line *cmd_line, \
char *delimiter, t_info *info, int i);
int		search_for_redirection(t_info *info);

//*** SIGNAL_UTILS.C ***
void	disable_signals(void);
void	enable_signals(void);

//*** SIGNAL.C ***
void	signal_inside_heredoc(int signum);
void	signal_outside_heredoc(int signum);
void	enable_signals_minishell(void);
void	signal_child(int signum);
void	signal_parent(int signum);

//*** SPLIT_TOKEN.C *** 
int	little_split_token(t_info *info, int i);
void	routine_split_token(t_info *info);
void	split_token(char *token, t_info *info);

//*** UTILS_1.C ***
t_token	*ft_lstnew_token(char *content);
t_token	*ft_lstlast_token(t_token *lst);
void	ft_lstaddback_token(t_token **alst, t_token *new);
void	trim_space(t_info *info, char *set);
void	remove_inside_quote(t_info *info);

//*** UTILS_2.C **
void	lst_print_token(t_info *info);
void	ft_lstdelone_token(t_token *lst, void (*del)(void *));
int		ft_lstsize_token(t_token *lst);
void	ft_lstclear_token(t_token **lst, void (*del) (void *));
void	print_error_cd(t_info *info, char *str);

//*** UTILS_3.C **
char	simple_or_double(char *token);
int		how_many(t_info *info, char *str, char c);
void	skip_space(t_info *info);
char	**split_path(char **env);
int		search_next_pipe(t_info *info);

//*** UTILS_BUILTINS.C ***
void	remove_quote(t_token *token_list);
// void	quote_remover(t_command_line **cmd_line);
void	quote_remover(t_info *info);
void	del_empty_node(t_token **token);
int		is_n(t_token *node);
char	*until_chr(char *str, char c);

//*** UTILS_EXECUTION.C ***
void	put_back_default_std(t_info *info);
void	exec_one_command(t_command_line cmd_line, t_info *info);
void	do_redirection(t_command_line cmd_line, t_info *info);
void	one_command_or_builtin(t_command_line *cmd_line, t_info *info);

//*** UTILS_EXPANSION.C ***
void	chop_chop(char **str, char **tab, char **env, int i);
void	find_expansion(char **str, char **tab, char **env);
void	delimiter_finder(t_info *info, char *delimiter, int fd[]);
void	free_inside_heredoc(int fd, t_info *info);

///*** UTILS_PRINT.C ***
void	print_struct(t_command_line *cmd_line, t_info *info);
void	print_double_pointer(char **str);

//*** UTILS_REDIRECTION.C ***
void	delete_tokens(t_token **list);
int		is_redirection(t_token *list);
void	delete_redirection_tokens(t_token *list_token, t_token **list_addr);
void	input_redirection(t_command_line *cmd_line, char *infile);
void	manage_heredoc_fds(t_info *info, t_command_line *cmd_line, int *fd);

//*** VAR_EXPANSION.C *** 
char	*new_expanded_variable(int i, char *str, char **env);
char	*env_variable(char *str, int *i);
void	locate_expansion(char **str, char **env, t_info *info, char *end);
void	var_expansion(t_command_line *cmd_line, t_info	*info);

char	*remove_matching_quote(char *str);

#endif