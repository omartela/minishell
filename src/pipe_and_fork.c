/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_and_fork.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 21:40:30 by irychkov          #+#    #+#             */
/*   Updated: 2024/09/07 23:58:00 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pipe_and_fork(t_shell *sh, t_pipes *pipes, int i, t_cmd *cmd)
{
	if (i < sh->num_cmds - 1)
	{
		if (pipe(pipes->fd[i]) == -1)
		{
			perror("pipe");
			return (1);
		}
	}
	pipes->pid[i] = fork();
	if (pipes->pid[i] == -1)
	{
		perror("fork");
		return (1);
	}
	if (pipes->pid[i] == 0)
	{
		child_io(cmd, pipes->fd, i, sh->num_cmds);
		execute_command(cmd, sh->envp);
		exit(1);
	}
	return (0);
}