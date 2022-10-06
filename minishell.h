#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include "Libft/libft.h"
#include "readline/readline.h"
#include "readline/history.h"
#include <termios.h>
#include <string.h>

typedef struct s_info
{
	char **envp;
	char **token;
	char *last_position;
}		t_info;

//*** MAIN.C ***
void	print_tab(char **tab);
char	*get_path(char *command, char **envp);
void	command_exeggutor(char *argv, char **envp);
void	echo(t_info *info);
char	*search_line(char **tab, char *search);
void	pwd(t_info *info);
void 	token_manager(t_info *info);
char 	simple_or_double(char *token);
void	split_token(char *token, t_info *info);
int 	main(int argc, char **argv, char **envp);

//*** SIGNAL.C ***
void	exit_terminal();
void    sig_handler(int signum);
void    signal_modified();
void 	disable_echo();

//Builtins

// void	pwd(char **envp);
// void	echo(char **tab_token);
