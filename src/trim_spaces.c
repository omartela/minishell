/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trim_spaces.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 12:36:54 by irychkov          #+#    #+#             */
/*   Updated: 2024/09/16 17:25:26 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*trim_spaces(char *str)
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
	return (str);
}
