/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute-builtin-exit-cd-export-env.c               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omartela <omartela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 13:14:22 by omartela          #+#    #+#             */
/*   Updated: 2024/10/24 13:14:34 by omartela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	execute_exit_builtin(t_shell *sh, t_cmd *cmd, int is_in_pipe)
{
	if (is_in_pipe)
		exit(exit_shell(sh, cmd));
	else
		return (exit_shell(sh, cmd));
}

int	execute_export_builtin(t_shell *sh, t_cmd *cmd, int is_in_pipe)
{
	if (is_in_pipe)
		return (handle_in_pipe(export, sh, cmd));
	else
		return (handle_not_in_pipe(export, sh, cmd));
}

int	execute_env_builtin(t_shell *sh, t_cmd *cmd, int is_in_pipe)
{
	if (is_in_pipe)
		return (handle_in_pipe(env, sh, cmd));
	else
		return (handle_not_in_pipe(env, sh, cmd));
}

int	execute_cd_builtin(t_shell *sh, t_cmd *cmd, int is_in_pipe)
{
	if (is_in_pipe)
		return (handle_in_pipe(cd, sh, cmd));
	else
		return (handle_not_in_pipe(cd, sh, cmd));
}
