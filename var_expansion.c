/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expansion.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cemenjiv <cemenjiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/26 09:58:57 by cemenjiv          #+#    #+#             */
/*   Updated: 2022/09/27 15:28:46 by cemenjiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void	*ft_calloc(size_t count, size_t size);
size_t	ft_strlen(const char *str);
char	*ft_strdup(const char *s1);
void	ft_bzero(void *s, size_t n);
void	*ft_memset(void *b, int c, size_t len);
char	*ft_strnstr(const char *haystack, const char *needle, size_t len);
char	*ft_strjoin(char const *s1, char const *s2);
size_t	ft_strlcpy(char *dst, const char *src, size_t dst_size);
size_t	ft_strlcat(char *dst, const char *src, size_t dst_size);
int	    ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_substr(char const *s, unsigned int start, size_t len);

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char			*rtn;
	unsigned int	index;

	index = 0;
	rtn = (char *)malloc (sizeof (char) * (len + 1));
	if (!(s && rtn))
		return (NULL);
	while (index < len && start < ft_strlen(s))
		rtn[index++] = s[start++];
	rtn[index] = 0;
	return (rtn);
}


int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	index;

	index = 0;
	if (!s1)
		return (*(unsigned char *)s2);
	if (n == 0)
		return (index);
	while ((*s1 && *s2) && (index < n))
	{
		if (*s1 != *s2)
			return (*(unsigned char *)s1 - *(unsigned char *)s2);
		s1++;
		s2++;
		index++;
	}
	if (index == n)
	{
		s1--;
		s2--;
	}	
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}



size_t	ft_strlcat(char *dst, const char *src, size_t dst_size)
{
	size_t	index;

	index = 0;
	while (*dst && index < dst_size)
	{
		index++;
		dst++;
	}
	while (*src && (index + 1) < dst_size)
	{
		*dst++ = *src++;
		index++;
	}
	if (index < dst_size)
		*dst = 0;
	while (*src++)
		index++;
	return (index);
}


size_t	ft_strlcpy(char *dst, const char *src, size_t dst_size)
{
	size_t	index;

	index = 0;
	if (!(dst || src))
		return (0);
	while (src[index] && (index + 1) < dst_size)
	{	
		dst[index] = src[index];
		index++;
	}
	if (dst_size != 0)
		dst[index] = 0;
	while (src[index])
		index++;
	return (index);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*rtn;
	size_t	len;

	len = ft_strlen(s1) + ft_strlen(s2) + 1;
	rtn = malloc(len);
	if (!rtn)
		return (NULL);
	ft_strlcpy(rtn, s1, len);
	ft_strlcat(rtn, s2, len);
	return (rtn);
}


char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{	
	char	*ptr_need;
	char	*ptr_hays;
	size_t	len_need;

	len_need = ft_strlen(needle);
	if (!*needle)
		return ((char *)haystack);
	if (!ft_strlen(haystack) || len < len_need)
		return (NULL);
	len -= --len_need;
	while (len-- && *haystack)
	{
		ptr_need = (char *) needle;
		ptr_hays = (char *) haystack;
		while (*ptr_hays && *ptr_need == *ptr_hays)
		{
			ptr_need++;
			ptr_hays++;
		}
		if (*ptr_need == 0)
			return ((char *) haystack);
		haystack++;
	}
	return (NULL);
}

void	*ft_memset(void *b, int c, size_t len)
{
	unsigned char	*ptrb;

	ptrb = b;
	while (len-- > 0)
		*ptrb++ = (unsigned char)c;
	return (b);
}

void	ft_bzero(void *s, size_t n)
{
	ft_memset(s, 0, n);
}

void	*ft_calloc(size_t count, size_t size)
{
	void	*rtn;

	rtn = malloc(count * size);
	if (!rtn)
		return (NULL);
	ft_bzero(rtn, count * size);
	return (rtn);
}

size_t	ft_strlen(const char *str)
{
	size_t	index;

	if (!str)
		return (0);
	index = 0;
	while (str[index])
		index++;
	return (index);
}

char	*ft_strdup(const char *s1)
{
	char	*rtn;
	int		len;
	int		index;

	index = 0;
	len = ft_strlen(s1);
	rtn = ft_calloc(len + 1, sizeof(char));
	if (!rtn)
		return (0);
	while (len--)
		rtn[index++] = *s1++;
	rtn[index] = 0;
	return (rtn);
}


/* Gerer les cas d'erreurs:
- (FAIT) Si le premier caractere du token n'est pas '$', ne rien faire. Le token, reste tel quel.
- (FAIT) Si le contenu suivant n'est pas exactement la meme expression (ex: ARG au lieu de ARGS), on retourne le token initial
- (FAIT) Si le contenu suivant le caractere '$' ne se trouve pas dans l'environnement, ne rien faire et retourner le token initial
- (FAIT) Si l'expansion est associe a null = ARGS = "", remplacer $ARGS par null;
- Si l'expansion ne se trouve pas dans ENV, output sera RIEN. Ex: echo $ARGS si ARGS n'est pas dans env. Output est rien.
- Si le contenu est $? = retourner l'exit number. Dans tous les autres cas, ne rien faire.  

Questions:
- Que faire si la variable existe dans le "set", mais elle n'existe pas dans env? On le gere ou pas?echo
- GEre
*/

char	*new_expanded_variable(int i, char *str, char **env)
{
	int j;

	j = 0;
	while (env[i][j] == str[j] && str[j])
		j++;
	str = &env[i][j];
	return (str);
}

char	*var_expansion(char **str, char **env)
{
	(void)env;
	char *str1;
	char *str2;
	char *str3;
	int i;
	int j;

	str3 = NULL;
	i = 0;
	j = 0;
	printf("Le token initial est %s\n", *str);
	while ((*str)[i] != '$')
		i++;
	if (i > 0)
		str3 = ft_substr(*str, 0, i);
	if ((*str)[i] == '$')	
	{
		i++;
		str1 = ft_strdup((*str + i));
		str2 = ft_strjoin(str1, "=");
		i = 0;
		while (env[i] != NULL && ft_strnstr(env[i], str2, ft_strlen(str2)) == 0)
		  	i++;
		printf("La variable i est d'une valeur de : %d\n", i);
		if (env[i] != NULL)
		{
			if (str3)
			{
				free(*str);	
				*str = ft_strjoin(str3, new_expanded_variable(i, str2, env));
			}
			else
				*str = new_expanded_variable(i, str2, env);
		}
		free(str1);
		free(str2);
		free(str3);
		printf("Le token final une fois l'expansion fait est: %s\n", *str);
	}
	return (*str);
}

int main(int argc, char **argv, char **env)
{
	(void)argc;
	(void)argv;
	char *str;
	
	str = malloc(10 * sizeof(char));
	strcpy(str, "$ARGS");
	var_expansion(&str, env);
	printf("Le token final est: %s\n", str);
	//free(str); // Seulement si la premiere lettre du string n'est pas $
}