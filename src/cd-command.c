/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd-command.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omartela <omartela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 16:06:06 by omartela          #+#    #+#             */
/*   Updated: 2024/09/05 12:06:28 by omartela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../include/minishell.h"

int	cd(t_shell *sh, char **args)
{
	/// Need to make more error handling for args...
	/// For example check that it is proper path etc...

	char    cwd[PATH_MAX];
    char    *oldpwd;
	char	*currentpwd;

    oldpwd = getcwd(cwd, sizeof(cwd));

	if (args[1] == NULL)
	{
		if (chdir(sh->homepath))
		{
			ft_putstr_fd("Error when changing directory", 2);
			return (1);
		}
		return (1);
	}
	else
	{
		if (chdir(args[1]))
		{
			ft_putstr_fd("Error when changing directory", 2);
			return (1);
		}
		if (oldpwd)
		{
			set_table(&sh->envp, "OLDPWD", oldpwd);
			set_table(&sh->local_shellvars, "OLDPWD", oldpwd);
		}
		else
		{
			ft_putstr_fd("Error when changing directory", 2);
			return (1);
		}
		currentpwd = getcwd(cwd, sizeof(cwd));
		if (currentpwd)
		{
			set_table(&sh->envp, "PWD", currentpwd);
			set_table(&sh->local_shellvars, "PWD", currentpwd);
		}
		else
		{
			ft_putstr_fd("Error when changing directory", 2);
			return (1);
		}
		return (0);
	}
	return (0);
}
