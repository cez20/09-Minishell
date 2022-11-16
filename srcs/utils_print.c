/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_print.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cemenjiv <cemenjiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/15 22:34:48 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/11/15 23:05:03 by cemenjiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h" 

void	print_struct(t_command_line *cmd_line, t_info *info)
{
	int	i;

	i = 0;
	while (i <= info->nb_of_pipe)
	{
		printf("Address of list_token is: %p\n", cmd_line[i].list_token);
		print_double_pointer(cmd_line[i].cmd_and_args);
		printf("Command is :%s\n", cmd_line[i].command);
		printf("Args is: %s\n", cmd_line[i].args);
		printf("Builtin number is: %d\n", cmd_line[i].builtin);
		printf("fd_in is:%d\n", cmd_line[i].fd_in);
		printf("fd_out is :%d\n", cmd_line[i].fd_out);
		printf("Error infile is: %s\n", cmd_line[i].error_infile);
		printf("Merge path_cmd is :%s\n", cmd_line[i].merge_path_cmd);
		i++;
	}
}

void	print_double_pointer(char **str)
{
	int	i;

	i = 0;
	if (!str)
		return ;
	while (str[i])
	{
		printf("The value of str[%d] is %s\n", i, str[i]);
		i++;
	}	
}
