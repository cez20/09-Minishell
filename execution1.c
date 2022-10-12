/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cemenjiv <cemenjiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 19:28:26 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/10/10 13:40:52 by cemenjiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
	int fd[3][2]; // 3 represent the number of 
	int i;
	for (i = 0; i < 3; i++)
	{
		if (pipe(fd[i]) < 0) //Not the right way of writing it. Normally, if a pipe has a problem, all others must be closed.
		{
			return 1; // pipe() function creates 3 pipes all at the same time. 
		}
	}
	
	//fork duplicates the fd, which are currently at 3
	int pid1 = fork(); //After this fork(), we have 3 more fds 
	if(pid1 < 0)
		return 2;
	//We are in child process #1
	if (pid1 == 0) 
	{
		close(fd[0][1]);
		close(fd[1][0]);
		close(fd[2][0]);
		close(fd[2][1]);
		int x = 0;
		if (read(fd[0][0], &x, sizeof(int)) < 0)
			return 3;
		x += 5;
		if (write(fd[1][1], &x, sizeof(int)) < 0)
		{
			return 4;
		}
		close(fd[0][0]);
		close(fd[1][1]);
		return 0;
	}
	//Calling the second fork, that copies the fds from parent process. Fork also duplicate all the memory from parent 
	int pid2 = fork();
	if (pid2 < 0)
	{
		return 5;
	}
	//We will now get inside the child proces
	if (pid2 == 0)
	{
		close(fd[0][0]);
		close(fd[0][1]);
		close(fd[1][1]);
		close(fd[2][0]);
		int x = 0; // Not the same x at in the other child process. 
		if (read(fd[1][0], &x, sizeof(int)) < 0)
			return 6;
		x += 5;
		if (write(fd[2][1], &x, sizeof(int)) < 0)
		{
			return 7;
		}
		close(fd[1][0]);
		close(fd[2][1]);
	}
	
	//Parent process: we want fd[0][1] fd[2][0]
	close(fd[0][0]);//Because we don't read from that pipe 
	close(fd[1][0]);
	close(fd[1][1]);
	close(fd[2][1]);
	int x = 0;
	if (write(fd[0][1], &x, sizeof(int)) < 0)
		return 8;
	if (read(fd[2][0], &x, sizeof(int)) < 0)
		return 9;
	printf("Result is %d\n", x);
	close(fd[0][1]);
	close(fd[2][0]);

	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);

	return (0);
}
