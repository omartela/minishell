/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin-pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 12:36:23 by omartela          #+#    #+#             */
/*   Updated: 2024/11/05 17:39:51 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	pwd(t_shell *sh)
{
	char	cwd[PATH_MAX];
	char	*str;

	str = getcwd(cwd, sizeof(cwd));
	if (str)
	{
		ft_putstr_fd(str, 1);
		write(1, "\n", 1);
		return (0);
	}
	str = expand(sh->envp, "PWD");
	if (str)
	{
		if (!*str)
			ft_putstr_fd("getcwd: cannot access directories: \
No such file or directory\n", 2);
		else
		{
			ft_putstr_fd(str, 1);
			write(1, "\n", 1);
			return (0);
		}
	}
	return (1);
}
