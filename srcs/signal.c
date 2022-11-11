/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slavoie <slavoie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/16 09:10:15 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/11/11 16:40:45 by slavoie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

//I need to free eveything that cause a segfault 
int	exit_terminal(void)
{
    printf(" exit\n");
	exit (EXIT_SUCCESS);
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
void sig_handler(int signum)
{
	if (signum == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
        // printf("Minishell$>");
		rl_redisplay();
	}
}

/* 
    SIGINT = equivalent of Ctrl+C, SIGQUIT = equivalent of Ctrl+\
    The signal() function defines a particular action to be realized
    when a signal is encountered. For example, when Ctrl+C is pressed
    The function sig_handler is immediately called whereas with the other
    signal, it is ignored*/
void	signal_modified(void)
{
	signal(SIGINT, &sig_handler);
	//signal(SIGQUIT, SIG_IGN);
}

// This function essentially disable the ECHOCTL function
// that normally prints out ^C when Ctrl+C signal is entered on
// keyboard. We take current terminal setting with tcgetattr and
// then modify the attributes c_lflag to disable ECHOCTL and then
// apply change with tcgetattr
void	disable_echo(void)
{
	struct termios	attributes;

	tcgetattr(STDIN_FILENO, &attributes);
	attributes.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &attributes);
}
