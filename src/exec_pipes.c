/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 14:25:13 by irychkov          #+#    #+#             */
/*   Updated: 2024/09/06 18:26:34 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	close_pipes_in_parent(t_shell *sh, int fd[][2])
{
	int	i;

	i = 0;
	while (i < sh->num_cmds - 1)
	{
		if (fd[i][0] != -1)
			close(fd[i][0]);
		if (fd[i][1] != -1)
			close(fd[i][1]);
		i++;
	}
}

static void	wait_for_children(t_shell *sh, pid_t *pid)
{
	int	i;

	i = 0;
	while (i < sh->num_cmds)
	{
		waitpid(pid[i], NULL, 0);
		i++;
	}
}

static void	child_io(t_cmd *cmd, int fd[][2], int i, t_shell *sh)
{
	if (cmd->infile)
	{
		dup2(cmd->fd_in, STDIN_FILENO);
		close(cmd->fd_in);
	}
	else if (i > 0)
	{
		close(fd[i - 1][1]);
		dup2(fd[i - 1][0], STDIN_FILENO);
		close(fd[i - 1][0]);
	}
	if (cmd->outfile)
	{
		dup2(cmd->fd_out, STDOUT_FILENO);
		close(cmd->fd_out);
	}
	else if (i < sh->num_cmds - 1)
	{
		close(fd[i][0]);
		dup2(fd[i][1], STDOUT_FILENO);
		close(fd[i][1]);
	}
}

int	execute_pipes(t_shell *sh)
{
	int		i;
	t_cmd	*cmd;
	int		fd[sh->num_cmds - 1][2];
	pid_t	pid[sh->num_cmds];

	i = 0;
	cmd = NULL;
	ft_memset(fd, -1, sizeof(int) * (sh->num_cmds - 1) * 2);
	while (sh->commands[i] != NULL)
	{
		if (init_cmd(&cmd, sh->commands[i], sh->envp) == 1)
			return (1);
		if (i < sh->num_cmds - 1)
		{
			if (pipe(fd[i]) == -1)
			{
				free_cmd(cmd);
				perror("pipe");
				return (1);
			}
		}
		pid[i] = fork();
		if (pid[i] == -1)
		{
			free_cmd(cmd);
			perror("fork");
			return (1);
		}
		if (pid[i] == 0)
		{//child
			child_io(cmd, fd, i, sh);
			execute_command(cmd, sh->envp);
		}
		free_cmd(cmd);
		i++;
	}
	//parent
	close_pipes_in_parent(sh, fd);
	wait_for_children(sh, pid);
	return (0);
}
