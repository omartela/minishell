/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute-builtin-commands.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omartela <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 14:29:37 by omartela          #+#    #+#             */
/*   Updated: 2024/10/23 15:45:21 by omartela         ###   ########.fr       */
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

int	execute_unset_builtin(t_shell *sh, t_cmd *cmd, int is_in_pipe)
{
	if (is_in_pipe)
		return (handle_in_pipe(unset, sh, cmd));
	else
		return (handle_not_in_pipe(unset, sh, cmd));
}

int	execute_pwd_builtin(t_shell *sh, t_cmd *cmd, int is_in_pipe)
{
	if (is_in_pipe)
	{
		if (pwd())
			exit_and_free(sh, cmd, 1);
		exit_and_free(sh, cmd, 0);
	}
	else
	{
		if (pwd())
		{
			sh->exit_status = 1;
			return (1);
		}
		sh->exit_status = 0;
		return (0);
	}
	return (0);
}

int	execute_echo_builtin(t_shell *sh, t_cmd *cmd, int is_in_pipe)
{
	if (!is_in_pipe)
	{
		if (echo(cmd->args))
		{
			sh->exit_status = 1;
			return (1);
		}
		sh->exit_status = 0;
		return (0);
	}
	else
	{
		if (echo(cmd->args))
			exit_and_free(sh, cmd, 1);
		exit_and_free(sh, cmd, 0);
	}
	return (0);
}

int	execute_builtin(t_shell *sh, t_cmd *cmd, int is_in_pipe)
{
	if (ft_strncmp(cmd->args[0], "exit\0", 5) == 0)
		return (execute_exit_builtin(sh, cmd, is_in_pipe));
	if (ft_strncmp(cmd->args[0], "export\0", 7) == 0)
		return (execute_export_builtin(sh, cmd, is_in_pipe));
	if (ft_strncmp(cmd->args[0], "env\0", 4) == 0)
		return (execute_env_builtin(sh, cmd, is_in_pipe));
	if (ft_strncmp(cmd->args[0], "cd\0", 3) == 0)
		return (execute_cd_builtin(sh, cmd, is_in_pipe));
	if (ft_strncmp(cmd->args[0], "unset\0", 6) == 0)
		return (execute_unset_builtin(sh, cmd, is_in_pipe));
	if (ft_strncmp(cmd->args[0], "pwd\0", 4) == 0)
		return (execute_pwd_builtin(sh, cmd, is_in_pipe));
	if (ft_strncmp(cmd->args[0], "echo\0", 5) == 0)
		return (execute_echo_builtin(sh, cmd, is_in_pipe));
	return (1);
}
