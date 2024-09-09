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

#include "minishell.h"

static int	count_env_vars(char **envp)
{
	int	count;

	count = 0;
	while (envp[count])
		count++;
	return (count);
}

char	**copy_envp(char **envp)
{
	int		i;
	int		env_count;
	char	**env_copy;

	i = 0;
	env_count = count_env_vars(envp);
	env_copy = (char **)malloc((env_count + 1) * sizeof(char *));
	if (!env_copy)
		return (NULL);
	while (i < env_count)
	{
		env_copy[i] = strdup(envp[i]);
		if (!env_copy[i])
		{
			free_array_back(env_copy, i);
			return (NULL);
		}
		i++;
	}
	env_copy[env_count] = NULL;
	return (env_copy);
}
