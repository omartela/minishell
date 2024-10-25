/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 14:25:13 by irychkov          #+#    #+#             */
/*   Updated: 2024/10/23 22:43:59 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	create_pipe_if_needed(t_shell *sh, int i)
{
	if (i < sh->num_cmds - 1)
	{
		if (pipe(sh->pipes->fd[i]) == -1)
		{
			error_sys("pipe failed\n");
			return (1);
		}
	}
	return (0);
}

static void	close_previous_pipes(t_shell *sh, int i)
{
	if (i > 0)
	{
		close(sh->pipes->fd[i - 1][0]);
		close(sh->pipes->fd[i - 1][1]);
	}
}

static int	pipe_and_fork(t_shell *sh, t_cmd *cmd, int i)
{
	int	is_build;

	if (create_pipe_if_needed(sh, i))
		return (1);
	is_build = is_builtin(cmd);
	if (sh->num_cmds == 1 && is_build == 1)
		return (exec_in_parent(sh, cmd));
	else if (is_build == -1)
		return (1);
	if (change_signal_handler())
	{
		error_sys("Changing signal handler failed\n");
		return (1);
	}
	sh->pipes->pid[i] = fork();
	if (sh->pipes->pid[i] == -1)
	{
		error_sys("fork failed\n");
		return (1);
	}
	if (sh->pipes->pid[i] == 0)
		exec_child(sh, cmd, i);
	close_previous_pipes(sh, i);
	return (0);
}

static int	execute_commands_in_pipes(t_shell *sh)
{
	int		i;
	t_cmd	*cmd;
	int		error_code;

	i = 0;
	cmd = NULL;
	while (sh->commands[i] != NULL)
	{
		if (init_cmd(&cmd, sh->commands[i], sh) == 1
			|| update_underscore(sh, cmd))
		{
			sh->exit_status = 1;
			return (1);
		}
		error_code = pipe_and_fork(sh, cmd, i);
		if (error_code || !cmd->is_continue)
		{
			free_cmd(&cmd);
			sh->exit_status = error_code;
			return (1);
		}
		free_cmd(&cmd);
		i++;
	}
	return (0);
}

void	execute_pipes(t_shell *sh)
{
	t_pipes	*pipes;

	pipes = ft_calloc(1, sizeof(t_pipes));
	if (!pipes)
	{
		error_sys("malloc failed for t_pipes\n");
		sh->exit_status = 1;
		return ;
	}
	sh->pipes = pipes;
	if (init_pipes(sh->pipes, sh->num_cmds) == 1)
	{
		error_sys("malloc failed for init_pipes\n");
		sh->exit_status = 1;
		return ;
	}
	if (execute_commands_in_pipes(sh))
		return ;
	wait_for_children(sh->pipes, sh);
}
