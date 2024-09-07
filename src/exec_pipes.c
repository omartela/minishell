/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 14:25:13 by irychkov          #+#    #+#             */
/*   Updated: 2024/09/08 00:37:33 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	close_pipes_in_parent(t_pipes *pipes, int num_cmds)
{
	int	i;

	i = 0;
	while (i < num_cmds - 1)
	{
		if (pipes->fd[i][0] != -1)
			close(pipes->fd[i][0]);
		if (pipes->fd[i][1] != -1)
			close(pipes->fd[i][1]);
		i++;
	}
}

static void	wait_for_children(t_pipes *pipes, int num_cmds)
{
	int	i;

	i = 0;
	while (i < num_cmds)
	{
		waitpid(pipes->pid[i], NULL, 0);
		i++;
	}
}

int	execute_pipes(t_shell *sh)
{
	int		i;
	t_cmd	*cmd;
	t_pipes	pipes;

	i = 0;
	cmd = NULL;
	if (init_pipes(&pipes, sh->num_cmds) == 1)
		return (1);
	while (sh->commands[i] != NULL)
	{
		if (init_cmd(&cmd, sh->commands[i], sh->envp) == 1)
			return (1);
		if (pipe_and_fork(sh, &pipes, i, cmd) != 0)
		{
			free_cmd(cmd);
			return (1);
		}
		free_cmd(cmd);
		i++;
	}
	//parent
	close_pipes_in_parent(&pipes, sh->num_cmds);
	wait_for_children(&pipes, sh->num_cmds);
	free_pipes(&pipes, sh->num_cmds);
	return (0);
}
