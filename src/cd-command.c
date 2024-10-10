/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd-command.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omartela <omartela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 16:06:06 by omartela          #+#    #+#             */
/*   Updated: 2024/10/09 23:29:39 by omartela         ###   ########.fr       */
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
	char	*path;

	path = NULL;
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
		if (args[2])
			return (show_error_return(1, args[0], "too many arguments"));
		if (ft_strncmp(args[1], "-\0", 2) == 0)
		{
			path = expand(sh->envp, "OLDPWD");
			if (!path)
				return (show_error_return(1, args[1], "cd - failed"));
			if (chdir(path) == -1)
			{
				free(path);
				return (show_error_return(1, args[1], "Not a directory"));
			}
			free(path);
		}
		else if (access(args[1], F_OK) == -1)
			return (show_error_return(1, args[1], "No such file or directory"));
		else if (access(args[1], R_OK) == -1)
			return (show_error_return(1, args[1], "Permission denied"));
		else if (chdir(args[1]) == -1)
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
