/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_args.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 12:44:50 by irychkov          #+#    #+#             */
/*   Updated: 2024/09/04 13:15:50 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
The function handles only quotes and spaces as separators.
It returns an array of strings, each string is a separate argument.
The function allocates memory for the array and for each string.
The array is NULL-terminated. If the allocation fails, the function returns NULL.
*/

static size_t	ft_strcounter(char const *s, char c)
{
	size_t	elements;
	int		in_quotes;
	char	quote_type;

	elements = 0;
	in_quotes = 0;
	while (*s)
	{
		while (*s == c && !in_quotes)
			s++;
		if (*s == '\0')
			break ;
		elements++;
		while (*s && (in_quotes || *s != c))
		{
			if ((*s == '\'' || *s == '\"') && !in_quotes)
			{
				in_quotes = 1;
				quote_type = *s;
			}
			else if (*s == quote_type && in_quotes)
				in_quotes = 0;
			s++;
		}
	}
	return (elements);
}

static void	free_array(char **array, size_t i)
{
	while (i > 0)
	{
		i--;
		free(array[i]);
	}
	free(array);
}

static char	**ft_helper(char const *s, char c, size_t i, char **result)
{
	const char	*start;
	int			in_quotes;
	char		quote_type;

	in_quotes = 0;
	while (*s)
	{
		while (*s == c && !in_quotes)
			s++;
		if (*s == '\0')
			break ;
		start = s;
		while (*s && (in_quotes || *s != c))
		{
			if ((*s == '\'' || *s == '\"') && !in_quotes)
			{
				in_quotes = 1;
				quote_type = *s;
			}
			else if (*s == quote_type && in_quotes)
				in_quotes = 0;
			s++;
		}
		result[i] = (char *)malloc(sizeof(char) * (s - start + 1));
		if (!result[i])
		{
			free_array(result, i);
			return (NULL);
		}
		ft_strlcpy(result[i], start, s - start + 1);
		i++;
	}
	result[i] = NULL;
	return (result);
}

char	**ft_split_args(char const *s, char c)
{
	size_t		i;
	char		**result;

	i = 0;
	result = (char **)malloc(sizeof(char *) * (ft_strcounter(s, c) + 1));
	if (!result || !s)
		return (NULL);
	return (ft_helper(s, c, i, result));
}
