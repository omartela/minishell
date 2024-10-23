/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute-builtin-commands.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omartela <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 14:29:37 by omartela          #+#    #+#             */
/*   Updated: 2024/10/23 14:30:24 by omartela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int handle_in_pipe(int (*builtin_func)(t_shell *, char **), t_shell *sh, t_cmd *cmd)
{
	if (builtin_func(sh, cmd->args))
		exit_and_free(sh, cmd, 1);
	exit_and_free(sh, cmd, 0);
	return (1);  // This won't actually be reached because of `exit()`.
}

int handle_not_in_pipe(int (*builtin_func)(t_shell *, char **), t_shell *sh, t_cmd *cmd)
{
	if (builtin_func(sh, cmd->args))
	{
		sh->exit_status = 1;
		return (1);
	}
	sh->exit_status = 0;
	return (0);
}

// Handle exit built-in
int execute_exit_builtin(t_shell *sh, t_cmd *cmd, int is_in_pipe)
{
	if (is_in_pipe)
		exit(exit_shell(sh, cmd));
	else
		return (exit_shell(sh, cmd));
}

// Handle export built-in
int execute_export_builtin(t_shell *sh, t_cmd *cmd, int is_in_pipe)
{
	if (is_in_pipe)
		return handle_in_pipe(export, sh, cmd);
	else
		return handle_not_in_pipe(export, sh, cmd);
}

// Handle env built-in
int execute_env_builtin(t_shell *sh, t_cmd *cmd, int is_in_pipe)
{
	if (is_in_pipe)
		return handle_in_pipe(env, sh, cmd);
	else
		return handle_not_in_pipe(env, sh, cmd);
}

// Handle cd built-in
int execute_cd_builtin(t_shell *sh, t_cmd *cmd, int is_in_pipe)
{
	if (is_in_pipe)
		return handle_in_pipe(cd, sh, cmd);
	else
		return handle_not_in_pipe(cd, sh, cmd);
}

// Handle unset built-in
int execute_unset_builtin(t_shell *sh, t_cmd *cmd, int is_in_pipe)
{
	if (is_in_pipe)
		return handle_in_pipe(unset, sh, cmd);
	else
		return handle_not_in_pipe(unset, sh, cmd);
}

// Handle pwd built-in
int execute_pwd_builtin(t_shell *sh, t_cmd *cmd, int is_in_pipe)
{
	if (is_in_pipe)
		return handle_in_pipe(pwd, sh, cmd);
	else
		return handle_not_in_pipe(pwd, sh, cmd);
}

// Handle echo built-in
int execute_echo_builtin(t_shell *sh, t_cmd *cmd, int is_in_pipe)
{
	if (is_in_pipe)
		return handle_in_pipe(echo, sh, cmd);
	else
		return handle_not_in_pipe(echo, sh, cmd);
}

int	execute_builtin(t_shell *sh, t_cmd *cmd, int is_in_pipe)
{
	if (ft_strncmp(cmd->args[0], "exit\0", 5) == 0)
		return execute_exit_builtin(sh, cmd, is_in_pipe);
	if (ft_strncmp(cmd->args[0], "export\0", 7) == 0)
		return execute_export_builtin(sh, cmd, is_in_pipe);
	if (ft_strncmp(cmd->args[0], "env\0", 4) == 0)
		return execute_env_builtin(sh, cmd, is_in_pipe);
	if (ft_strncmp(cmd->args[0], "cd\0", 3) == 0)
		return execute_cd_builtin(sh, cmd, is_in_pipe);
	if (ft_strncmp(cmd->args[0], "unset\0", 6) == 0)
		return execute_unset_builtin(sh, cmd, is_in_pipe);
	if (ft_strncmp(cmd->args[0], "pwd\0", 4) == 0)
		return execute_pwd_builtin(sh, cmd, is_in_pipe);
	if (ft_strncmp(cmd->args[0], "echo\0", 5) == 0)
		return execute_echo_builtin(sh, cmd, is_in_pipe);

	return 1;  // Default case if no built-in command matches.
}

