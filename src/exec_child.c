/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 16:21:56 by irychkov          #+#    #+#             */
/*   Updated: 2024/10/23 16:38:04 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_child_input(t_shell *sh, t_cmd *cmd, int **fd, int i)
{
	if (cmd->infile || cmd->here_doc)
	{
		if (i > 0)
		{
			close(fd[i - 1][0]);
			close(fd[i - 1][1]);
		}
		if (dup2(cmd->fd_in, STDIN_FILENO) == -1)
			error_dup(sh, cmd);
		close(cmd->fd_in);
	}
	else if (i > 0)
	{
		close(fd[i - 1][1]);
		if (dup2(fd[i - 1][0], STDIN_FILENO) == -1)
			error_dup(sh, cmd);
		close(fd[i - 1][0]);
	}
}

static void	handle_child_output(t_shell *sh, t_cmd *cmd, int **fd, int i)
{
	if (cmd->outfile)
	{
		if (i < sh->num_cmds - 1)
		{
			close(fd[i][0]);
			close(fd[i][1]);
		}
		if (dup2(cmd->fd_out, STDOUT_FILENO) == -1)
			error_dup(sh, cmd);
		close(cmd->fd_out);
	}
	else if (i < sh->num_cmds - 1)
	{
		close(fd[i][0]);
		if (dup2(fd[i][1], STDOUT_FILENO) == -1)
			error_dup(sh, cmd);
		close(fd[i][1]);
	}
}

static void	child_io(t_shell *sh, t_cmd *cmd, int i)
{
	handle_child_input(sh, cmd, sh->pipes->fd, i);
	handle_child_output(sh, cmd, sh->pipes->fd, i);
}

void	exec_child(t_shell *sh, t_cmd *cmd, int i)
{
	int	is_build;

	rl_clear_history();
	if (reset_signals(sh))
		exit(1);
	parse_redirections(sh, cmd, 1);
	child_io(sh, cmd, i);
	is_build = is_builtin(cmd);
	if (is_build == 1)
	{
		if (execute_builtin(sh, cmd, 1))
			exit(1);
		else
			exit(0);
	}
	else if (is_build == -1)
		exit(1);
	execute_command(sh, cmd, sh->envp);
	exit(1);
}
