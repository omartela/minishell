/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin-env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 17:40:47 by omartela          #+#    #+#             */
/*   Updated: 2024/10/08 17:11:33 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	env(t_shell *shell, char **args)
{
	int	argc;
	int	i;

	argc = 0;
	i = 0;
	while (args[argc])
		++argc;
	if (argc == 1)
	{
		while (shell->envp[i])
		{
			ft_printf("%s\n", shell->envp[i]);
			++i;
		}
	}
	return (0);
}
