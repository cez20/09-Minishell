#include "minishell.h"

void	remove_quote(char **str)
{
	char	*temp;
	char	

	if (simple_or_double(*str) != 32)
	{
		temp = *str;
		*str = ft_substr(*str, 1, ft_strlen(*str) - 2);
		free(temp);
	}
	
}

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
		remove_quote(&info->token[i]);
		printf("%s", info->token[i++]);
	}
	if (ft_strncmp(info->token[1], "-n", 2))
		printf("\n");
}