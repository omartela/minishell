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
	(*envp)[0] = "PATH=/usr/bin:/bin";
	(*envp)[1] = NULL;
}
