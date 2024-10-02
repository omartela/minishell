/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_builtin_single.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 20:44:45 by irychkov          #+#    #+#             */
/*   Updated: 2024/10/02 21:21:51 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	save_fds(int *saved_stdin, int *saved_stdout)
{
	*saved_stdout = dup(STDOUT_FILENO);
	if (*saved_stdout == -1)
	{
		error_sys("dup failed\n");
		return (1);
	}
	*saved_stdin = dup(STDIN_FILENO);
	if (*saved_stdin == -1)
	{
		error_sys("dup failed\n");
		close(*saved_stdout);
		return (1);
	}
	return (0);
}

static int	restore_fds(int saved_stdin, int saved_stdout)
{
	if (dup2(saved_stdout, STDOUT_FILENO) == -1)
	{
		error_sys("dup2 failed\n");
		close(saved_stdout);
		close(saved_stdin);
		return (1);
	}
	if (dup2(saved_stdin, STDIN_FILENO) == -1)
	{
		error_sys("dup2 failed\n");
		close(saved_stdout);
		close(saved_stdin);
		return (1);
	}
	close(saved_stdout);
	close(saved_stdin);
	return (0);
}

int	setup_fds_for_builtin(t_cmd *cmd, int *saved_stdin, int *saved_stdout)
{
	if (save_fds(saved_stdin, saved_stdout))
		return (1);
	if (cmd->infile && dup2(cmd->fd_in, STDIN_FILENO) == -1)
	{
		error_sys("dup2 failed\n");
		return (restore_fds(*saved_stdin, *saved_stdout));
	}
	if (cmd->outfile && dup2(cmd->fd_out, STDOUT_FILENO) == -1)
	{
		error_sys("dup2 failed\n");
		return (restore_fds(*saved_stdin, *saved_stdout));
	}
	return (0);
}

int	execute_builtin_with_fds(t_shell *sh, t_cmd *cmd,
								int saved_stdin, int saved_stdout)
{
	if (execute_builtin(sh, cmd))
		return (restore_fds(saved_stdin, saved_stdout));
	cmd->is_continue = 0;
	return (restore_fds(saved_stdin, saved_stdout));
}
