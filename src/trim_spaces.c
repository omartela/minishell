/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trim_spaces.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 12:36:54 by irychkov          #+#    #+#             */
/*   Updated: 2024/10/01 11:12:55 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	trim_leading_trailing_spaces(char *str)
{
	int	len;
	int	start;
	int	end;

	start = 0;
	end = ft_strlen(str) - 1;
	while (str[start] && (str[start] == ' ' || str[start] == '\t'
			|| str[start] == '\n'))
		start++;
	while (end >= start && (str[end] == ' ' || str[end] == '\t'
			|| str[end] == '\n'))
		end--;
	len = end - start + 1;
	if (len < 0)
		ft_memmove(str, str + start, len);
	str[len] = '\0';
}

static void	reduce_multiple_spaces(char *str)
{
	int i = 0;
	int j = 0;

	while (str[i])
	{
		if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n')
			str[j++] = str[i];
		else if ((str[i] == ' ' || str[i] == '\t' || str[i] == '\n')
			&& (j == 0 || str[j - 1] != ' '))
			str[j++] = ' ';
		i++;
	}
	str[j] = '\0';
}

char	*trim_spaces(char *str)
{
	trim_leading_trailing_spaces(str);
	reduce_multiple_spaces(str);
	return (str);
}
