/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 11:56:47 by irychkov          #+#    #+#             */
/*   Updated: 2024/09/24 17:16:10 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*copy_with_quotes(char *start, size_t len)
{
	char	*result;

	result = malloc(len + 1);
	if (!result)
		return (NULL);
	ft_strlcpy(result, start, len + 1);
	return (result);
}

static size_t	len_without_quotes(char *str, size_t len)
{
	size_t	i;
	size_t	count;
	int		in_quotes;
	char	quote_type;

	in_quotes = 0;
	quote_type = 0;
	i = 0;
	count = 0;
	while (i < len)
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
		{
			count++;
		}
		i++;
	}
	return (count);
}

static char	*copy_string(char *start, size_t len, t_split_opts *opts)
{
	char	*result;
	size_t	new_len;
	size_t	i;
	size_t	j;
	int		in_quotes;
	char	quote_type;

	in_quotes = 0;
	quote_type = 0;
	i = 0;
	j = 0;
	if (opts->keep_quotes)
	{
		result = copy_with_quotes(start, len);
		return (result);
	}
	new_len = len_without_quotes(start, len);
	result = malloc(new_len + 1);
	if (!result)
		return (NULL);
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
		{
			result[j++] = start[i];
		}
		i++;
	}
	result[j] = '\0';
	return (result);
}

static char	**ft_helper(char *s, size_t i, char **result, t_split_opts *opts)
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
		result[i] = copy_string(start, s - start, opts);
		if (!result[i])
		{
			free_array_back(&result, i);
			return (NULL);
		}
		i++;
	}
	result[i] = NULL;
	return (result);
}

char	**split_args_general(char *s, char c, int keep_quotes)
{
	size_t			i;
	char			**result;
	t_split_opts	opts;

	i = 0;
	opts.keep_quotes = keep_quotes;
	opts.delimiter = c;
	result = (char **)malloc(sizeof(char *) * (ft_strcounter(s, c) + 1));
	if (!result || !s)
		return (NULL);
	return (ft_helper(s, i, result, &opts));
}
