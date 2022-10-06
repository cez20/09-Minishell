#include "minishell.h"

void	pwd(t_info *info)
{
	char *pwd;

	pwd = search_line(info->envp, "PWD=");
	pwd = ft_strchr(pwd, '=');
	printf("%s\n", ++pwd);
}

void	echo(t_info *info)
{
	int i;

	if (!ft_strncmp(info->token[1], "-n", 2))
		i = 2;
	else
		i = 1;

	while(info->token[i])
	{
		printf("%s ", info->token[i++]);
	}
	if (ft_strncmp(info->token[1], "-n", 2))
		printf("\n");
}