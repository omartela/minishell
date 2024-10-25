/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd-command.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 16:06:06 by omartela          #+#    #+#             */
/*   Updated: 2024/10/22 18:47:25 by omartela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	cd_to_home(t_shell *sh, char **args)
{
	char	*path;
	char	cwd[PATH_MAX];
	char	*oldpwd;

	oldpwd = getcwd(cwd, sizeof(cwd));
	if (set_oldpwd(sh, oldpwd))
		return (1);
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
	if (set_currentpwd(sh))
		return (1);
	return (0);
}

static int	cd_to_oldpwd(t_shell *sh, char **args)
{
	char	*path;
	char	cwd[PATH_MAX];
	char	*oldpwd;

	oldpwd = getcwd(cwd, sizeof(cwd));
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
	if (set_oldpwd(sh, oldpwd))
		return (1);
	ft_printf("%s\n", path);
	free(path);
	if (set_currentpwd(sh))
		return (1);
	return (0);
}

static int	cd_to_path(t_shell *sh, char **args)
{
	char	cwd[PATH_MAX];
	char	*oldpwd;

	oldpwd = getcwd(cwd, sizeof(cwd));
	if (set_oldpwd(sh, oldpwd))
		return (1);
	if (chdir(args[1]) == -1)
		return (show_error_return(1, args[1], "Not a directory"));
	if (set_currentpwd(sh))
		return (1);
	return (0);
}

int	cd(t_shell *sh, char **args)
{
	if (args[1] == NULL)
	{
		if (cd_to_home(sh, args))
			return (1);
		return (0);
	}
	else
	{
		if (args[2])
			return (show_error_return(1, args[0], "too many arguments"));
		if (ft_strncmp(args[1], "-\0", 2) == 0)
		{
			if (cd_to_oldpwd(sh, args))
				return (1);
			return (0);
		}
		else if (access(args[1], F_OK) == -1)
			return (show_error_return(1, args[1], "No such file or directory"));
		else if (access(args[1], R_OK) == -1)
			return (show_error_return(1, args[1], "Permission denied"));
		else if (cd_to_path(sh, args))
			return (1);
	}
	return (0);
}
