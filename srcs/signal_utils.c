/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slavoie <slavoie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/19 20:39:07 by slavoie           #+#    #+#             */
/*   Updated: 2022/11/19 20:42:10 by slavoie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
