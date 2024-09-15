/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_args_helper.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 12:14:41 by irychkov          #+#    #+#             */
/*   Updated: 2024/09/15 12:59:40 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**split_args_without_quotes(char *s, char c)
{
	return (split_args_general(s, c, 0));
}

char	**split_args_with_quotes(char *s, char c)
{
	return (split_args_general(s, c, 1));
}

size_t	ft_strcounter(char *s, char c)
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
