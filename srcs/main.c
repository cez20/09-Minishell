/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slavoie <slavoie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/15 14:50:27 by slavoie           #+#    #+#             */
/*   Updated: 2022/10/22 10:56:04 by slavoie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// char	*get_path(char *command, char **envp)
// {
// 	char	**path_list;
// 	char	*full_path;
// 	char	*temp;
// 	int		i;

// 	i = 0;
// 	while (envp[i] != NULL && ft_strnstr(envp[i], "PATH=", 5) == 0)
// 		i++;
// 	printf("path = %s\n", envp[i]);
// 	if (envp[i] == NULL)
// 		return (NULL);
// 	path_list = ft_split(envp[i] + 5, ':');
// 	i = 0;
// 	while (path_list[i])
// 	{
// 		temp = ft_strjoin(path_list[i], "/");
// 		full_path = ft_strjoin(temp, command);
// 		free(temp);
// 		printf("full_path = %s\n", full_path);
// 		if (access(full_path, F_OK | X_OK) == 0)
// 			return (full_path);
// 		free(full_path);
// 		i++;
// 	}
// 	table_flip(path_list);
// 	return (0);
// }

// void	command_exeggutor(char *argv, char **envp)
// {
// 	char	**command;
// 	char	*full_path;
// 	int		i;

// 	i = -1;
// 	command = ft_split(argv, ' ');
// 	print_tab(command);
// 	if (access(command[0], F_OK | X_OK) == 0)
// 		full_path = command[0];
// 	else
// 		full_path = get_path(command[0], envp);
// 	printf("%s\n", full_path);
// 	if (!full_path)
// 	{
// 		while (command[++i])
// 			free(command[i]);
// 		free(command);
// 		yo_its_wrong("pas de path\n");
// 	}
// 	if (execve(full_path, command, envp) == -1)
// 		yo_its_wrong("echec exec\n");
// }

/*
	cherche la ligne (char *line) dans le tableau (char **tab) et la renvoie
	si rien n'est trouvé, renvoie NULL 
*/
char	*search_line(char **tab, char *line)
{
	while(*tab)
	{
		if (ft_strnstr(*tab, line, ft_strlen(line)))
			return(*tab);
		tab++;
	}
	return (NULL);
}

/*
	exécute le builtin associer à la première commande
*/
void token_manager(t_info *info)
{
	if (!ft_strncmp(info->list_token->token, "echo", 4))
		echo(info);
	if (!ft_strncmp(info->list_token->token, "pwd", 3))
		pwd(info);
	if (!ft_strncmp(info->list_token->token, "env", 3))
		print_tab(info->envp);
	if (!ft_strncmp(info->list_token->token, "cd", 2))
		cd(info);
	if (!ft_strncmp(info->list_token->token, "exit", 4))
		exit_terminal();
	if (!ft_strncmp(info->list_token->token, "export", 6))
		export(info);
	

}

/*
	Vérifier la valeur du char à l'adresse du char pointeur "token"
	renvoie la valeur du char si c'est '' ou ""
	sinon renvoie la valeur de l'espace soit '32' 
*/
char simple_or_double(char *token)
{
	if (*token == 34 || *token == 39)
		return (*token);
	return (32);
}

int	how_many(char *str, char c)
{
	int i;

	i = 0;
	while (*str)
	{
		if (*str == c)
			i++;
		str++;
	}
	return (i);
}

/*
	Avance le pointeur jusqu'à la prochaine chose à interpréter
*/
void skip_space(t_info *info)
{
	while(is_white_space(*info->last_position))
	{
		if (*info->last_position == '|')
			return ;
		info->last_position++;
		if (info->list_token)
			ft_lstlast_token(info->list_token)->space_flag = 1;
	}
	// printf("last_position skip= %c\n", *info->last_position);
}

/*
	Cherche la prochaine occurence de "c" et renvoie le token
*/
char	*search_another_one(char *str, char c, t_info *info)
{
	char	*token;
	char	*start;
	int		len;

	start = str;
	len = 0;
	str++;
	len++;
	while (*str != c)
	{
		if (*str == '\0' || (c == 32 && (*str == 34 || *str == 39)))
		{
			info->last_position = str;
			token = ft_substr(start, 0, len);
			return (token);
		}
		str++;
		len++;
	}
	if (c != 32)
	{
		info->last_position = ++str;
		len++;
	}
	else
		info->last_position = str;
	// printf("last_position search = %c\n", *info->last_position);

	token = ft_substr(start, 0, len);
	return (token);
}

// char	*get_command(char *token)
// {



// }

// char *get_token(t_info *info)
// {
// 	int	i;

// 	i = 0;
// 	skip_space(info);
// 	if (*info->last_position != 39 && *info->last_position != 34)
		




// }

void	split_token(char *token, t_info *info)
{
	int i;
	int x;

	i = 0;
	x = 0;
	info->last_position = token;
	info->command_lines = malloc(sizeof(t_command_line) * info->nb_of_pipe + 1);

	while (*info->last_position)
	{
		skip_space(info);
		if (*info->last_position == '|')
		{
			info->command_lines[i].list_token = info->list_token;
			info->list_token = NULL;
			info->last_position++;
			i++;
		}
		ft_lstaddback_token(&info->list_token, ft_lstnew_token(search_another_one(info->last_position, simple_or_double(info->last_position), info)));
		// info->token = tab_join(info->token, search_another_one(info->last_position, simple_or_double(info->last_position), info));
		skip_space(info);
	}
	info->command_lines[i].list_token = info->list_token;
	// info->list_token = NULL;
	x = i;
	i = 0;
	while (x >= i)
	{
		printf("i = %d\n", i);
		lst_print_token(&info->command_lines[i++].list_token);
	}
	// print_tab(info->token);
	// lst_print_token(&info->list_token);
}

void	init(t_info *info, char **envp)
{
	info->envp = tabcpy(envp);
	// info->envp = envp;
	info->list_token = NULL;
	info->flag_quote = 0;
	info->last_position = NULL;
	info->infile = -1;
	info->outfile = -1;
	info->nb_of_pipe = 0;
	// info->meta = ft_split(METACHARACTER, ' ');
	// info->redirection = NULL;
}

//Fonction qui permet de reinitialiser certaines infos de la struct 
void	reinit(t_info *info)
{
	if(info->infile != -1)
		close(info->infile);
	if (info->outfile != -1)
		close(info->outfile);
	info->nb_of_pipe = 0;
}


// char	*create_prompt(t_info *info)
// {
// 	char	pwd[4096];
// 	int	i;
// 	int	start;

// 	i = 0;
// 	start = 0;
// 	getcwd(pwd, ft_strl);

// 	while (pwd[i])
// 	{
// 		if (pwd[i] == '/')
// 			start = i;
// 		i++;
// 	}
// 	info->prompt = ft_substr(pwd, start, i);
// 	info->prompt = ft_strjoin(info->prompt, " Minishell> ");
// 	return (info->prompt);
// }



int main(int argc, char **argv, char **envp)
{
	char *line;
	t_info	*info;

	info = malloc(sizeof(t_info));
	init(info, envp);

	printf("Let's go ça part !\n");
	//print_tab(envp); //Prints the whole env command 
	disable_echo();
	while(1 && argc && argv && envp)
	{
		
		signal_modified();
		line = readline("Minishell$> ");
		if (line)
			add_history(line); 
		else 
			exit_terminal();
		how_many(line, '|');
		split_token(line, info);
		var_expansion(info->list_token, envp);
		if (info->list_token)
			token_manager(info);
		// lst_print_token(&info->list_token);

		redirection(info);
		execution(info);
		// chdir
		// command_exeggutor(line, envp);
		// free(info->prompt);
		// free(info->token);
		// ft_lstclear_token(&info->list_token, del);
		free(line);
		ft_lstclear_token(&info->list_token, free);
		reinit(info);

		// info->token = ft_calloc(sizeof(char **), 1);
	}
	// Le free ne peut pas être dans la while, sinon elle créer un segfault à la prochaine itération
	free (info); // Liberer le pointeur declare en debut de fonction main.  
	return (0);
}
