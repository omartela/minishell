/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_update_args.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 14:04:53 by irychkov          #+#    #+#             */
/*   Updated: 2024/09/09 16:30:59 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* #include "minishell.h" */
#include "..//libft/libft.h"
#include <stdio.h>

size_t	ft_strlen(const char	*str)
{
	size_t	i;

	i = 0;
	while (str[i] != '\0')
	{
		i++;
	}
	return (i);
}

static size_t	ft_redirect_counter(char const *s)
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
			spaces++;
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

static char	*ft_add_spaces(char const *s)
{
	size_t	len;
	size_t	spaces_needed;
	char	*new_str;
	char	*dest;
	int		in_quotes;
	char	quote_type;

	in_quotes = 0;
	len = ft_strlen(s);
	spaces_needed = ft_redirect_counter(s);
	new_str = malloc(len + spaces_needed + 1); // protect
	dest = new_str;
	if (!new_str)
		return NULL;
	while (*s)
	{
		if (((*s == '<') || (*s == '>')) && !in_quotes)
		{
			if (dest > new_str && *(dest - 1) != ' ' && *(dest - 1) != '<' && *(dest - 1) != '>')
				*dest++ = ' ';
			*dest++ = *s;
			if (*(s + 1) && *(s + 1) != ' ' && *(s + 1) != '<' && *(s + 1) != '>')
				*dest++ = ' ';
		}
		else
		{
			if ((*s == '\'' || *s == '\"') && !in_quotes)
			{
				in_quotes = 1;
				quote_type = *s;
			}
			else if (in_quotes && *s == quote_type)
				in_quotes = 0;
			*dest++ = *s;
		}
		s++;
	}

	*dest = '\0';
	return (new_str);
}

int	main(void)
{
	char str[] = "<<<>><cat'<sda,d>'<><<>";
	char *new_str = ft_add_spaces(str);
	if (new_str)
	{
		printf("Original string: \"%s\"\n", str);
		printf("Modified string: \"%s\"\n", new_str);
		free(new_str);
	}
	return (0);
}