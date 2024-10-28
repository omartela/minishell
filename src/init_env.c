/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 11:07:15 by irychkov          #+#    #+#             */
/*   Updated: 2024/10/28 17:42:58 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	alloc_tables(t_shell *sh, char ***c_envp, char ***l_vars, size_t i)
{
	*c_envp = ft_calloc(i + 1, sizeof(char *));
	*l_vars = ft_calloc(i + 1, sizeof(char *));
	if (!c_envp || !l_vars)
	{
		if (c_envp)
			free(c_envp);
		if (l_vars)
			free(l_vars);
		sh->envp = NULL;
		sh->local_shellvars = NULL;
		error_sys("Copy environment failed..\n");
		return (1);
	}
	return (0);
}

static int	copy_vars(char **envp, char **c_envp, char **l_vars, size_t sarray)
{
	size_t	i;

	i = 0;
	while (i < sarray)
	{
		c_envp[i] = ft_strdup(envp[i]);
		if (!c_envp[i])
		{
			free_array_back(c_envp, i);
			free_array_back(l_vars, i);
			error_sys("Copy environment failed..\n");
			return (1);
		}
		l_vars[i] = ft_strdup(envp[i]);
		if (!l_vars[i])
		{
			free_array_back(l_vars, i);
			free_array_back(c_envp, i + 1);
			error_sys("Copy environment failed..\n");
			return (1);
		}
		++i;
	}
	return (0);
}

int	copy_env(char **envp, t_shell *shell)
{
	size_t	sarray;
	char	**copied_envp;
	char	**local_shellvars;

	sarray = 0;
	copied_envp = NULL;
	local_shellvars = NULL;
	sarray = calculate_table_size(&envp);
	if (alloc_tables(shell, &copied_envp, &local_shellvars, sarray))
		return (1);
	if (copy_vars(envp, copied_envp, local_shellvars, sarray))
		return (1);
	copied_envp[sarray] = NULL;
	local_shellvars[sarray] = NULL;
	if (sarray > 1)
		local_shellvars = sort_table(local_shellvars);
	shell->envp = copied_envp;
	shell->local_shellvars = local_shellvars;
	return (0);
}
