/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 11:07:15 by irychkov          #+#    #+#             */
/*   Updated: 2024/10/23 11:32:32 by omartela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	alloc_tables(t_shell *sh, char ***c_envp, char ***l_vars, size_t i)
{
	*c_envp = ft_calloc(i, sizeof(char *) + 1);
	*l_vars = ft_calloc(i, sizeof(char *) + 1);
	if (!c_envp || !l_vars)
	{
		if (c_envp)
			free(c_envp);
		if (l_vars)
			free(l_vars);
		sh->envp = NULL;
		sh->local_shellvars = NULL;
		error_sys("Copy environment failed..\n");
		exit(1);
	}
}

static void	copy_vars(char **envp, char **c_envp, char **l_vars, size_t sarray)
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
			exit(1);
		}
		l_vars[i] = ft_strdup(envp[i]);
		if (!l_vars[i])
		{
			free_array_back(l_vars, i);
			free_array_back(c_envp, i + 1);
			error_sys("Copy environment failed..\n");
			exit(1);
		}
		++i;
	}
}

void	copy_env(char **envp, t_shell *shell)
{
	size_t	sarray;
	char	**copied_envp;
	char	**local_shellvars;

	sarray = 0;
	// think what to do if envp is "empty" or does not exist
	sarray = calculate_table_size(&envp);
	alloc_tables(shell, &copied_envp, &local_shellvars, sarray);
	copy_vars(envp, copied_envp, local_shellvars, sarray);
	copied_envp[sarray] = NULL;
	local_shellvars[sarray] = NULL;
	local_shellvars = sort_table(local_shellvars);
	shell->envp = copied_envp;
	shell->local_shellvars = local_shellvars;
}
