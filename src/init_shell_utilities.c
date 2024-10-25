/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_shell_utilities.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 19:46:07 by irychkov          #+#    #+#             */
/*   Updated: 2024/10/24 19:48:27 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	initialize_default_env(char ***envp)
{
	*envp = malloc(sizeof(char *) * 2);
	if (!*envp)
	{
		error_sys("Failed to allocate memory for environment\n");
		exit(1);
	}
	(*envp)[0] = ft_strdup("PATH=/usr/bin:/bin");
	(*envp)[1] = NULL;
}
