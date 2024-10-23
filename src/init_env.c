/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 11:07:15 by irychkov          #+#    #+#             */
/*   Updated: 2024/09/08 00:40:07 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	alloc_tables(t_shell *sh, char ***c_envp, char ***l_shvars, size_t i)
{
	*c_envp = ft_calloc(i, sizeof(char *) + 1);
	*l_shvars = ft_calloc(i, sizeof(char *) + 1);
	if (!c_envp || !l_shvars)
	{
		if (c_envp)
			free(c_envp);
		if (l_shvars)
			free(l_shvars);
		sh->envp = NULL;
		sh->local_shellvars = NULL;
		error_sys("Copy environment failed..\n");
		exit(1);
	}
}

void	copy_env(char **envp, t_shell *shell)
{
	size_t	sarray;
	char	**copied_envp;
	char	**local_shellvars;
	size_t	i;

	sarray = 0;
	i = 0;
	// think what to do if envp is "empty" or does not exist
	sarray = calculate_table_size(&envp);
	alloc_tables(shell, &copied_envp, &local_shellvars, sarray);
	i = 0;
	while (i < sarray)
	{
		copied_envp[i] = ft_strdup(envp[i]);
		if (!copied_envp[i])
		{
			free_array_back(copied_envp, i);
			free_array_back(local_shellvars, i);
			error_sys("Copy environment failed..\n");
			exit(1);
		}
		local_shellvars[i] = ft_strdup(envp[i]);
		if (!local_shellvars[i])
		{
			free_array_back(local_shellvars, i);
			free_array_back(copied_envp, i + 1);
			error_sys("Copy environment failed..\n");
			exit(1);
		}
		++i;
	}
	copied_envp[i] = NULL;
	local_shellvars[i] = NULL;
	local_shellvars = sort_table(local_shellvars);
	shell->envp = copied_envp;
	shell->local_shellvars = local_shellvars;
}
