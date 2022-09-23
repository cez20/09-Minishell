#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include "Libft/libft.h"
#include "readline/readline.h"
#include "readline/history.h"
#include <termios.h>

typedef struct s_info
{
	char **envp;
	char **token;
}		t_info;

//*** MAIN.C ***
void	print_tab(char **tab);
char	*get_path(char *command, char **envp);
void	command_exeggutor(char *argv, char **envp);
void	echo(char **tab_token);
char	*search_line(char **tab, char *search);
void	pwd(char **envp);
void 	token_manager(char **tab_token, char **envp);
char 	simple_or_double(char *token);
char	**split_token(char *token);
int 	main(int argc, char **argv, char **envp);

//*** SIGNAL.C ***
void	exit_terminal();
void    sig_handler(int signum);
void    signal_modified();
void 	disable_echo();
