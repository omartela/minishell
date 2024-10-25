/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_pipes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 23:28:36 by irychkov          #+#    #+#             */
/*   Updated: 2024/10/23 17:36:25 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	init_fds(int **fd, int num_cmds)
{
	int	i;

	i = 0;
	while (i < num_cmds - 1)
	{
		fd[i][0] = -1;
		fd[i][1] = -1;
		i++;
	}
}

static void	free_fds_on_failure(int ***fd, int i)
{
	while (i > 0)
	{
		i--;
		free(fd[i]);
		fd[i] = NULL;
	}
	free(fd);
	fd = NULL;
}

int	init_pipes(t_pipes *pipes, int num_cmds)
{
	int	i;

	i = 0;
	pipes->fd = ft_calloc((num_cmds - 1), sizeof(int *));
	if (!pipes->fd)
		return (1);
	while (i < num_cmds - 1)
	{
		pipes->fd[i] = ft_calloc(2, sizeof(int));
		if (!pipes->fd[i])
		{
			free_fds_on_failure(&pipes->fd, i);
			return (1);
		}
		i++;
	}
	pipes->pid = ft_calloc(num_cmds, sizeof(pid_t));
	if (!pipes->pid)
	{
		free_fds_on_failure(&pipes->fd, i);
		return (1);
	}
	init_fds(pipes->fd, num_cmds);
	return (0);
}

void	wait_for_children(t_pipes *pipes, t_shell *sh)
{
	int	i;
	int	status;
	int	signal_number;

	i = 0;
	status = 0;
	signal_number = 0;
	while (i < sh->num_cmds)
	{
		waitpid(pipes->pid[i], &status, 0);
		if (WIFEXITED(status))
			sh->exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			signal_number = WTERMSIG(status);
			sh->exit_status = 128 + signal_number;
			if (sh->exit_status == 139 && sh->num_cmds == 1)
				ft_putstr_fd("Segmentation fault (core dumped)\n", 1);
		}
		i++;
	}
}
