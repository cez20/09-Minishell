#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include "Libft/libft.h"
#include "readline/readline.h"
#include "readline/history.h"
#include <termios.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

typedef struct s_token
{
	char	*token;
	int		flag_quote;
	int		space_flag;


	struct s_token *next;
}				t_token;


typedef struct s_info
{
	char **envp;
	char **token;
	t_token	*list_token;
	int flag_quote;
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
void 	redirection(char **token, int *infile, int *outfile)

// *** EXECUTION1.C ***    CHANGER LE NOM DE CECI UNE FOIS TERMINE
void	remove_extra_quote(char **token, char quote);


//Builtins

// void	pwd(char **envp);
// void	echo(char **tab_token);
void	ft_lstaddback_token(t_token **alst, t_token *new);
t_token	*ft_lstnew_token(char *content);
void lst_print_token(t_token **list);
void	ft_lstclear_token(t_token **lst, void (*del) (void *));

