/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd-command.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 16:06:06 by omartela          #+#    #+#             */
/*   Updated: 2024/10/10 18:38:31 by irychkov         ###   ########.fr       */
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
		path = expand(sh->envp, "HOME");
		if (!path)
		{
			error_sys("cd syserror: Allocation failed\n");
			return (1);
		}
		if (chdir(path) == -1)
		{
			free(path);
			return (show_error_return(1, args[0], "HOME not set"));
		}
		free(path);
		return (0);
	}
	else
	{
		if (args[2])
			return (show_error_return(1, args[0], "too many arguments"));
		if (ft_strncmp(args[1], "-\0", 2) == 0)
		{
			path = expand(sh->envp, "OLDPWD");
			if (!path)
			{
				error_sys("cd syserror: Allocation failed\n");
				return (1);
			}
			if (chdir(path) == -1)
			{
				free(path);
				return (show_error_return(1, args[0], "OLDPWD not set"));
			}
			ft_printf("%s\n", path);
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
			if (set_variables(sh, "OLDPWD", oldpwd))
			{
				error_sys("cd syserror: Setting oldpwd failed\n");
				return (1);
			}
		}
		else
		{
			error_sys("cd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory\n");
			return (1);
		}
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
			error_sys("cd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory\n");
			return (1);
		}
		return (0);
	}
	return (0);
}
