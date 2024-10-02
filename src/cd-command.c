/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd-command.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 16:06:06 by omartela          #+#    #+#             */
/*   Updated: 2024/10/02 22:22:52 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		if (access(args[1], F_OK) == -1)
			return (show_error_return(1, args[1], "No such file or directory"));
		if (access(args[1], R_OK) == -1)
			return (show_error_return(1, args[1], "Permission denied"));
		if (chdir(args[1]) == -1)
			return (show_error_return(1, args[1], "Not a directory"));
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
