/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin-export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omartela <omartela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 14:13:39 by omartela          #+#    #+#             */
/*   Updated: 2024/09/10 16:32:38 by omartela         ###   ########.fr       */
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
	while (arguments[argc])
		argc++;
	if (argc == 1)
	{
		while (shell->envp[i])
		{
			if (shell->envp[i + 1])
				ft_printf("declare -x %s \n", shell->envp[i]);
			else
				ft_printf("declare -x %s", shell->envp[i]);
		}
		return (0);
	}
	else if (arguments[0] && arguments[1])
	{
		equal = ft_strchr(arguments[1], '=');
		if (equal && (equal - 1) && (equal + 1) && (*(equal - 1) != '-') && (*(equal - 1) != '+'))
			temp = ft_split(arguments[1], '=');
		if (temp)
			return (set_env(shell, temp[0], temp[1]));
		else
			return (1);
	}
	else if (arguments[1])
		return (set_env(shell, arguments[1], NULL));
}
