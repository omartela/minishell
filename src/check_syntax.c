/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 19:27:15 by irychkov          #+#    #+#             */
/*   Updated: 2024/09/11 11:24:51 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_redirection_operator(char *str)
{
	if (ft_strncmp(str, ">", 1) == 0 || ft_strncmp(str, "<", 1) == 0
		|| ft_strncmp(str, ">>", 2) == 0 || ft_strncmp(str, "<<", 2) == 0)
		return (1);
	return (0);
}

int	syntax_check(char **commands)
{
	int	i;

	i = 0;
	while (commands[i])
	{
		if (ft_strlen(commands[i]) == 1 && commands[i][0] == ' ')
		{
			error_sys("minishell: syntax error near unexpected token `|'\n");
			return (1);
		}
		if (is_redirection_operator(commands[i]))
		{
			if (commands[i + 1] == NULL || (ft_strlen(commands[i + 1]) == 1 && commands[i + 1][0] == ' '))
			{
				printf("minishell: syntax error near unexpected token `%s`\n", commands[i]);
				return (1);
			}
		}
		i++;
	}
	return (0);
}