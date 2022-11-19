/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cemenjiv <cemenjiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/16 09:10:15 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/11/18 16:20:32 by slavoie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

//Fonction qui intercepte exit_code:
// Macro WIFEXITED retourne true si child a quitte correctement
// Macro WEXITSTATUS permet de transformer le status dans le bon int
// Macro WIFSIGNALED permet de capter si child a quitte avec signal
// WTERMSIG transforme status bon int. On ajoute 128 pour avoir le bon exit_code
int	get_exit_code(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (0);
}

//I need to free eveything that cause a segfault 
int	exit_terminal(t_info *info)
{
	// printf("\r");
	printf("\033[1A\e[0;32mMinishell$>\033[0m exit\n");
	// printf("	    exit");
	close(info->initial_stdin);
	close(info->initial_stdout);
	exit (info->exit_code);
}

/*
    sig_handler() function enumerated the different actions
    to be realized when SIGINT signal is sent to kernel. 
    1- Print newline on terminal
    2- rl_on_new_line tell terminal, that readline() function will be reading
    from this new line. This function normally following a \n
    3- rl_replace_line (char *text, int clear_undo): 
    if any text was entered prior to pressing Ctrl+C,
    rl_replace line takes the text entered (found in buffer) 
    and replace it by whatever is given as first argument.
    4- rl_redisplay changes what's display on screen to change for what
    is in rl_buffer.
*/

void	enable_signals_minishell(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	if (rl_on_new_line() == -1)
		exit(1);
	rl_replace_line("", 1);
}

void	signal_child(int signum)
{
	if (signum == SIGINT)
	{
		printf("\n");
		if (rl_on_new_line() == -1)
			exit(1);
		rl_replace_line("", 1);
	}
	else if (signum == SIGQUIT)
		printf("Quit: 3\n");
}

// void	signal_child(int signum)
// {
// 	if (signum == SIGINT)
// 		printf("\n");
// 	else if (signum == SIGQUIT)
// 		printf("Quit: 3\n");

// }

void	signal_parent(int signum)
{
	if (signum == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	enable_signals(void)
{
	struct termios	old;
	struct termios	new;

	tcgetattr(STDIN_FILENO, &old);
	new = old;
	new.c_lflag |= ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &new);
	signal(SIGINT, &signal_child);
	signal(SIGQUIT, &signal_child);
}

// This function essentially disable the ECHOCTL function
// that normally prints out ^C when Ctrl+C signal is entered on
// keyboard. We take current terminal setting with tcgetattr and
// then modify the attributes c_lflag to disable ECHOCTL and then
// apply change with tcgetattr
void	disable_signals(void)
{
	struct termios	old;
	struct termios	new;

	tcgetattr(STDIN_FILENO, &old);
	new = old;
	new.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &new);
	signal(SIGINT, &signal_parent);
	signal(SIGQUIT, SIG_IGN);
}
