/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute-builtin-commands-utilities.c               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omartela <omartela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 13:08:13 by omartela          #+#    #+#             */
/*   Updated: 2024/10/24 13:08:26 by omartela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	handle_in_pipe(int (*builtin_func)(t_shell *, char **), t_shell *sh, t_cmd *cmd)
{
	if (builtin_func(sh, cmd->args))
		exit_and_free(sh, cmd, 1);
	exit_and_free(sh, cmd, 0);
	return (1);
}

int	handle_not_in_pipe(int (*builtin_func)(t_shell *, char **), t_shell *sh, t_cmd *cmd)
{
	if (builtin_func(sh, cmd->args))
	{
		sh->exit_status = 1;
		return (1);
	}
	sh->exit_status = 0;
	return (0);
}
