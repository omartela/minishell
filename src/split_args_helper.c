/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_args_helper.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 12:14:41 by irychkov          #+#    #+#             */
/*   Updated: 2024/10/23 14:36:27 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*copy_with_quotes(char *start, size_t len)
{
	char	*result;

	result = ft_calloc(len + 1, sizeof(char));
	if (!result)
		return (NULL);
	ft_strlcpy(result, start, len + 1);
	return (result);
}

static void	copy_without_quotes(char *start, size_t len,
		char *result, size_t *j)
{
	int		in_quotes;
	char	quote_type;
	size_t	i;

	i = 0;
	in_quotes = 0;
	quote_type = 0;
	while (i < len)
	{
		if ((start[i] == '\'' || start[i] == '\"') && !in_quotes)
		{
			in_quotes = 1;
			quote_type = start[i];
		}
		else if (start[i] == quote_type && in_quotes)
		{
			in_quotes = 0;
			quote_type = 0;
		}
		else
			result[(*j)++] = start[i];
		i++;
	}
	result[*j] = '\0';
}

static size_t	len_without_quotes(char *str, size_t len)
{
	size_t	i;
	size_t	count;
	int		in_quotes;
	char	quote_type;

	in_quotes = 0;
	quote_type = 0;
	i = -1;
	count = 0;
	while (++i < len)
	{
		if ((str[i] == '\'' || str[i] == '\"') && !in_quotes)
		{
			in_quotes = 1;
			quote_type = str[i];
		}
		else if (str[i] == quote_type && in_quotes)
		{
			in_quotes = 0;
			quote_type = 0;
		}
		else
			count++;
	}
	return (count);
}

static char	*copy_string(char *start, size_t len, t_split_opts *opts)
{
	char	*result;
	size_t	new_len;
	size_t	j;

	j = 0;
	if (opts->keep_quotes)
	{
		result = copy_with_quotes(start, len);
		return (result);
	}
	new_len = len_without_quotes(start, len);
	result = ft_calloc(new_len + 1, sizeof(char));
	if (!result)
		return (NULL);
	copy_without_quotes(start, len, result, &j);
	return (result);
}

char	**split_args_helper(char *s, size_t i,
	char **result, t_split_opts *opts)
{
	char	*start;
	int		in_quotes;
	char	quote_type;

	in_quotes = 0;
	start = NULL;
	quote_type = 0;
	while (*s)
	{
		while (*s == opts->delimiter && !in_quotes)
			s++;
		if (*s == '\0')
			break ;
		start = s;
		while (*s && (in_quotes || *s != opts->delimiter))
			process_quotes(&s, &in_quotes, &quote_type);
		result[i++] = copy_string(start, s - start, opts);
		if (!result[i - 1])
		{
			free_array_back(result, i - 1);
			return (NULL);
		}
	}
	result[i] = NULL;
	return (result);
}
