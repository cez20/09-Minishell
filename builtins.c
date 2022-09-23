#include "minishell.h"

void	pwd(char **envp)
{
	char *pwd;

	pwd = search_line(envp, "PWD=");
	pwd = ft_strchr(pwd, '=');
	printf("%s\n", ++pwd);
}

void	echo(char **tab_token)
{
	int i;

	if (!ft_strncmp(tab_token[1], "-n", 2))
		i = 2;
	else
		i = 1;

	while(tab_token[i])
	{
		printf("%s ", tab_token[i++]);
	}
	if (ft_strncmp(tab_token[1], "-n", 2))
		printf("\n");
}