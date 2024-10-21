/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_pipes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 23:28:36 by irychkov          #+#    #+#             */
/*   Updated: 2024/10/13 23:02:34 by irychkov         ###   ########.fr       */
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

static void	free_fds_on_failure(int **fd, int i)
{
	while (i > 0)
	{
		i--;
		free(fd[i]);
	}
	free(fd);
}

int	init_pipes(t_pipes *pipes, int num_cmds)
{
	int	i;
	int z;

	i = 0;
	z = 0;
	pipes->fd = ft_calloc((num_cmds - 1), sizeof(int *));
	if (!pipes->fd)
		return (1);
	while (i < num_cmds - 1)
	{
		pipes->fd[i] = ft_calloc(2, sizeof(int));
		if (!pipes->fd[i])
		{
			free_fds_on_failure(pipes->fd, i);
			return (1);
		}
		i++;
	}
	pipes->pid = ft_calloc(num_cmds, sizeof(pid_t));
	if (!pipes->pid)
	{
		free_fds_on_failure(pipes->fd, i);
		return (1);
	}
	/* while (z < num_cmds)
	{
		ft_memset(&pipes->pid[z], 0, sizeof(pid_t));
		++z;
	} */ // Changed to ft_calloc so this memset should be unnecessary
	init_fds(pipes->fd, num_cmds);
	return (0);
}
