/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_shell_utilities.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 19:46:07 by irychkov          #+#    #+#             */
/*   Updated: 2024/10/28 17:48:39 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	initialize_default_env(char ***envp, int *allocated)
{
	char	**temp_envp;

	temp_envp = ft_split("PATH=/usr/bin:/bin", ' ');
	if (!temp_envp)
	{
		error_sys("Allocation failed\n");
		exit(1);
	}
	*envp = temp_envp;
	*allocated = 1;
}

void	initialize_env(t_shell *sh, char ***envp)
{
	int	allocated;

	allocated = 0;
	if (!*envp || !(*envp)[0])
		initialize_default_env(envp, &allocated);
	if (copy_env(*envp, sh))
	{
		if (allocated)
			free_array(envp);
		exit(1);
	}
	if (allocated)
		free_array(envp);
	*envp = sh->envp;
}
