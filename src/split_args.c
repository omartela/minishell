/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 11:56:47 by irychkov          #+#    #+#             */
/*   Updated: 2024/10/23 14:33:29 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	process_quotes(char **s, int *in_quotes, char *quote_type)
{
	if ((**s == '\'' || **s == '\"') && !(*in_quotes))
	{
		*in_quotes = 1;
		*quote_type = **s;
	}
	else if (**s == *quote_type && *in_quotes)
	{
		*in_quotes = 0;
	}
	(*s)++;
}

static size_t	ft_strcounter(char *s, char c)
{
	size_t	elements;
	int		in_quotes;
	char	quote_type;

	elements = 0;
	in_quotes = 0;
	quote_type = 0;
	while (*s)
	{
		while (*s == c && !in_quotes)
			s++;
		if (*s == '\0')
			break ;
		elements++;
		while (*s && (in_quotes || *s != c))
			process_quotes(&s, &in_quotes, &quote_type);
	}
	return (elements);
}

char	**split_args_general(char *s, char c, int keep_quotes)
{
	size_t			i;
	char			**result;
	t_split_opts	opts;

	i = 0;
	opts.keep_quotes = keep_quotes;
	opts.delimiter = c;
	result = (char **)ft_calloc((ft_strcounter(s, c) + 1), sizeof(char *));
	if (!result)
		return (NULL);
	return (split_args_helper(s, i, result, &opts));
}

char	**split_args_remove_quotes(char *s, char c)
{
	return (split_args_general(s, c, 0));
}

char	**split_args_leave_quotes(char *s, char c)
{
	return (split_args_general(s, c, 1));
}
