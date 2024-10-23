/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_add_spaces.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 14:04:53 by irychkov          #+#    #+#             */
/*   Updated: 2024/10/23 15:11:19 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	ft_redirect_count(char *s)
{
	size_t	spaces;
	int		in_quotes;
	char	quote_type;

	spaces = 0;
	in_quotes = 0;
	while (*s)
	{
		if (((*s == '<') || (*s == '>')) && !in_quotes)
		{
			spaces += 2;
			while ((*s == '<') || (*s == '>'))
				s++;
			continue ;
		}
		if ((*s == '\'' || *s == '\"') && !in_quotes)
		{
			in_quotes = 1;
			quote_type = *s;
		}
		else if (in_quotes && *s == quote_type)
			in_quotes = 0;
		s++;
	}
	return (spaces);
}

static void	check_quotes_and_type(char *s, char *quote_type, char **dest, int *in_quotes)
{
	if ((*s == '\'' || *s == '\"') && !*in_quotes)
	{
		*in_quotes = 1;
		*quote_type = *s;
	}
	else if (*in_quotes && *s == *quote_type)
		*in_quotes = 0;
	**dest = *s;
	(*dest)++;
}


static void handle_redirection_spaces(char **dest, char *new_str, char **s)
{
	if (*dest > new_str && *(*dest - 1) != ' ' && *(*dest - 1) != '<' && *(*dest - 1) != '>')
	{
		**dest = ' ';
		(*dest)++;
	}
	**dest = **s;
	(*dest)++;
	if (*(*s + 1) && *(*s + 1) != ' ' && *(*s + 1) != '<' && *(*s + 1) != '>')
	{
		**dest = ' ';
		(*dest)++;
	}
}

char	*add_spaces(char *s)
{
	char	*new_str;
	char	*dest;
	int		in_quotes;
	char	quote_type;

	in_quotes = 0;
	new_str = ft_calloc(ft_strlen(s) + ft_redirect_count(s) + 1, sizeof(char));
	dest = new_str;
	if (!new_str)
		return (NULL);
	while (*s)
	{
		if (((*s == '<') || (*s == '>')) && !in_quotes)
			handle_redirection_spaces(&dest, new_str, &s);
		else
			check_quotes_and_type(s, &quote_type, &dest, &in_quotes);
		s++;
	}
	*dest = '\0';
	return (new_str);
}
