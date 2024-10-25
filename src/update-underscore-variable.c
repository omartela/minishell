/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update-underscore-variable.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 09:27:13 by omartela          #+#    #+#             */
/*   Updated: 2024/10/23 10:57:23 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	update_underscore_command(t_shell *sh, t_cmd *cmd)
{
	if (set_table(&sh->envp, "_", cmd->args[0]))
	{
		error_sys("Error when updating _ \n");
		return (1);
	}
	return (0);
}

static int	update_underscore_argument(t_shell *sh, t_cmd *cmd)
{
	int	i;

	i = 0;
	while (cmd->args[i])
		++i;
	if (set_table(&sh->envp, "_", cmd->args[i - 1]))
	{
		error_sys("Error when updating _ \n");
		return (1);
	}
	return (0);
}

int	update_underscore(t_shell *sh, t_cmd *cmd)
{
	int	i;

	i = 0;
	while (cmd->args[i])
		++i;
	if (i > 1)
		return (update_underscore_argument(sh, cmd));
	else
		return (update_underscore_command(sh, cmd));
}
