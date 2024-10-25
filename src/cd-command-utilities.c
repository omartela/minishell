/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd-command-utilities.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 12:58:56 by omartela          #+#    #+#             */
/*   Updated: 2024/10/24 18:47:40 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	set_currentpwd(t_shell *sh)
{
	char	cwd[PATH_MAX];
	char	*currentpwd;

	currentpwd = getcwd(cwd, sizeof(cwd));
	if (currentpwd)
	{
		if (set_variables(sh, "PWD", currentpwd))
		{
			error_sys("cd syserror: Setting pwd failed\n");
			return (1);
		}
	}
	else
	{
		error_sys("cd: error retrieving current directory: \
getcwd: cannot access parent directories: \
No such file or directory\n");
		return (1);
	}
	return (0);
}

int	set_oldpwd(t_shell *sh, char *oldpwd)
{
	if (oldpwd)
	{
		if (set_variables(sh, "OLDPWD", oldpwd))
		{
			error_sys("cd syserror: Setting oldpwd failed\n");
			return (1);
		}
	}
	else
	{
		error_sys("cd: error retrieving current directory: \
getcwd: cannot access parent directories: \
No such file or directory\n");
		return (1);
	}
	return (0);
}
