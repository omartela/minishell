/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_in_parent.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 16:40:57 by irychkov          #+#    #+#             */
/*   Updated: 2024/10/23 17:09:41 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	restore_fds(int saved_stdin, int saved_stdout)
{
	int	error;

	error = 0;
	if (dup2(saved_stdout, STDOUT_FILENO) == -1)
	{
		error_sys("dup2 failed to restore STDOUT\n");
		error = 1;
	}
	if (dup2(saved_stdin, STDIN_FILENO) == -1)
	{
		error_sys("dup2 failed to restore STDIN\n");
		error = 1;
	}
	close(saved_stdout);
	close(saved_stdin);
	return (error);
}

static int	save_stdio(t_cmd *cmd)
{
	cmd->saved_std[1] = dup(STDOUT_FILENO);
	if (cmd->saved_std[1] == -1)
	{
		error_sys("dup failed\n");
		return (1);
	}
	cmd->saved_std[0] = dup(STDIN_FILENO);
	if (cmd->saved_std[0] == -1)
	{
		error_sys("dup failed\n");
		close(cmd->saved_std[1]);
		return (1);
	}
	return (0);
}

static int	handle_redirects(t_cmd *cmd)
{
	if (cmd->infile)
	{
		if (dup2(cmd->fd_in, STDIN_FILENO) == -1)
		{
			error_sys("dup2 failed\n");
			restore_fds(cmd->saved_std[0], cmd->saved_std[1]);
			return (1);
		}
		close(cmd->fd_in);
	}
	if (cmd->outfile)
	{
		if (dup2(cmd->fd_out, STDOUT_FILENO) == -1)
		{
			error_sys("dup2 failed\n");
			restore_fds(cmd->saved_std[0], cmd->saved_std[1]);
			return (1);
		}
		close(cmd->fd_out);
	}
	return (0);
}

int	exec_in_parent(t_shell *sh, t_cmd *cmd)
{
	int	error_code;

	error_code = parse_redirections(sh, cmd, 0);
	if (error_code)
		return (error_code);
	if (save_stdio(cmd))
		return (1);
	if (handle_redirects(cmd))
		return (1);
	if (execute_builtin(sh, cmd, 0))
	{
		restore_fds(cmd->saved_std[0], cmd->saved_std[1]);
		return (1);
	}
	if (restore_fds(cmd->saved_std[0], cmd->saved_std[1]))
		return (1);
	cmd->is_continue = 0;
	return (0);
}
