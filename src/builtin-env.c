/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin-env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 17:40:47 by omartela          #+#    #+#             */
/*   Updated: 2024/11/04 11:03:39 by irychkov         ###   ########.fr       */
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
		return (0);
	}
	else
		return (show_error_return(1, args[0], "does \
not handle options or arguments!"));
}
