/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin-export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omartela <omartela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 14:13:39 by omartela          #+#    #+#             */
/*   Updated: 2024/09/11 11:59:47 by omartela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	export(t_shell *shell, char **arguments)
{
	int		argc;
	char	**temp;
	int		i;
	char	*equal;

	argc = 0;
	i = 0;
	temp = NULL;
	equal = NULL;
	while (arguments[argc])
		argc++;
	if (argc == 1)
	{
		while (shell->local_shellvars[i])
		{
			ft_printf("declare -x %s \n", shell->local_shellvars[i]);
			++i;
		}
		return (0);
	}
	else if (arguments[0] && arguments[1] && ft_strchr(arguments[1], '=') && (*(ft_strchr(arguments[1], '=') + 1)) != '\0')
	{
		equal = ft_strchr(arguments[1], '=');
		if (equal && (*(equal - 1) != '-') && (*(equal - 1) != '+'))
		{
			temp = ft_split(arguments[1], '=');
			if (temp)
			{
				if (!set_table(&shell->envp, temp[0], temp[1]))
				{
					if (!set_table(&shell->local_shellvars, temp[0], temp[1]))
					{
						sort_table(shell->local_shellvars);
						return (0);
					}
					else
						return (1);
				}
				else
					return (1);
			}
			else
				return (1);
		}
	}
	else if (arguments[0] && arguments[1])
	{
		if (!add_table(&shell->local_shellvars, arguments[1], NULL))
		{
			sort_table(shell->local_shellvars);
			return (0);
		}
		else
			return (1);
	}
	return (0);
}
