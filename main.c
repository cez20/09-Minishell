#include "minishell.h"

void	print_tab(char **tab)
{
	while (*tab)
	{
		printf("str = %s\n", *tab);
		tab++;
	}
}



char	*get_path(char *command, char **envp)
{
	char	**path_list;
	char	*full_path;
	char	*temp;
	int		i;

	i = 0;
	while (envp[i] != NULL && ft_strnstr(envp[i], "PATH=", 5) == 0)
		i++;
	printf("path = %s\n", envp[i]);
	if (envp[i] == NULL)
		return (NULL);
	path_list = ft_split(envp[i] + 5, ':');
	i = 0;
	while (path_list[i])
	{
		temp = ft_strjoin(path_list[i], "/");
		full_path = ft_strjoin(temp, command);
		free(temp);
		printf("full_path = %s\n", full_path);
		if (access(full_path, F_OK | X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	table_flip(path_list);
	return (0);
}

void	command_exeggutor(char *argv, char **envp)
{
	char	**command;
	char	*full_path;
	int		i;

	i = -1;
	command = ft_split(argv, ' ');
	print_tab(command);
	if (access(command[0], F_OK | X_OK) == 0)
		full_path = command[0];
	else
		full_path = get_path(command[0], envp);
	printf("%s\n", full_path);
	if (!full_path)
	{
		while (command[++i])
			free(command[i]);
		free(command);
		yo_its_wrong("pas de path\n");
	}
	if (execve(full_path, command, envp) == -1)
		yo_its_wrong("echec exec\n");
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
		printf("%s", tab_token[i++]);
	}
	if (ft_strncmp(tab_token[1], "-n", 2))
		printf("\n");
	
}

void token_manager(char **tab_token)
{
	// printf("tab_token[0] = %s\n", tab_token[0]);
	// printf("tab_token[1] = %s\n", tab_token[1]);
	// printf("len = %zu\n", ft_strlen(tab_token[0]));
	if (!ft_strncmp(tab_token[0], "echo", 4))
		echo(tab_token);
}

char simple_or_double(char *token)
{
	while(*token)
	{
		// printf("char = %d\n", *token);
		if (*token == 34|| *token == 39)
			return (*token);
		token++;
	}
	return (0);
}

char	**split_token(char *token)
{
	char	**tab;

	if (simple_or_double(token))
		tab = ft_split(token, simple_or_double(token));
	else
		tab = ft_split(token, ' ');

	return (tab);
}

int main(int argc, char **argv) // char **envp)
{
	char *line;
	char **tab_token;

	printf("Let's go ça part !\n");
	// print_tab(envp);
	
	while(1 && argc && argv)
	{
		line = readline("Minishell$>");
		add_history(line);
		rl_redisplay();
		// printf("token = %s", line);
		tab_token = split_token(line);

		token_manager(tab_token);
		// command_exeggutor(line, envp);
		free(line);
	}
}