/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd-command.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omartela <omartela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 16:06:06 by omartela          #+#    #+#             */
/*   Updated: 2024/09/05 12:06:28 by omartela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../include/minishell.h"

int	cd_command(char *path, t_shell *sh)
{
	if (path == NULL)
	{
		if (chdir(sh->homepath))
			printf("Error when changing directory");
	}
	else
	{
		if (chdir(path))
			printf("Error when changing directory");
	}
}
