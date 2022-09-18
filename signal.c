/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cemenjiv <cemenjiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/16 09:10:15 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/09/16 09:24:04 by cemenjiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    sig_handler(int signum)
{
	if (signum == SIGINT)
    {
        printf("\n"); 
		rl_on_new_line();
        rl_replace_line("", 0); 
        rl_redisplay(); 
    }
}

// SIGINT = equivalent of Ctrl+C, SIGQUIT = equivalent of Ctrl+\
// SIGSTP = equivalent of CTrl+Z (latter no supposed to be managed)
// The signal() function defines a particular action to be realized
// when a signal is encounted. For example, when Ctrl+C is pressed
// The function sig_handler is immediately called whereas with the other
// signal, it is ignored
void    signal_modified()
{
    signal(SIGINT, &sig_handler);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);    
}