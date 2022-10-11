#include "minishell.h"

void	remove_quote(char **str)
{
	char	*temp;
	char	chr;

	chr = simple_or_double(*str);

	// printf("%c\n", chr);
	// printf("%c\n", str[0][ft_strlen(*str) - 2]);

	if (str[0][ft_strlen(*str) - 1] == chr && chr != 32)
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

/*
		hypothèse pour gérer "" et '' coller 

		flag dans la struct si oui ou non il y avait des quotes donc une struct token

		flag dans la fonction

		ne pas enlever les quotes, mais simplement ne pas les afficher 

*/




void	echo(t_info *info)
{
	int i;

	if (!ft_strncmp(info->token[1], "-n", 2))
		i = 2;
	else
		i = 1;

	while(info->token[i])
	{
		// printf("%c\n", info->token[i][0]);
		if (info->token[i][0] == (34 | 39)) 
		{
			remove_quote(&info->token[i]);
			printf("%s", info->token[i++]);
		}
		else
		{
			// remove_quote(&info->token[i]);
			printf("%s ", info->token[i++]);
		}
	}
	if (ft_strncmp(info->token[1], "-n", 2))
		printf("\n");
}