/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 14:25:13 by irychkov          #+#    #+#             */
/*   Updated: 2024/10/14 16:26:57 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	child_io(t_cmd *cmd, int **fd, int i, int num_cmds)
{
	if (cmd->infile || cmd->here_doc)
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
	else if (i < num_cmds - 1)
	{
		close(fd[i][0]);
		dup2(fd[i][1], STDOUT_FILENO);
		close(fd[i][1]);
	}
}

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

static int	pipe_and_fork(t_shell *sh, t_pipes *pipes, int i, t_cmd *cmd)
{
	int	error_code;

	error_code = 0;
	if (i < sh->num_cmds - 1)
	{
		if (pipe(pipes->fd[i]) == -1)
		{
			error_sys("pipe failed\n");
			return (1);
		}
	}
	if (sh->num_cmds == 1 && is_builtin(cmd))
	{
		error_code = parse_redirections(sh ,cmd, 0);
		if (error_code)
			return (error_code);
		int saved_stdout = dup(STDOUT_FILENO);
		if (saved_stdout == -1)
		{
			error_sys("dup failed\n");
			return (1);
		}

		int saved_stdin = dup(STDIN_FILENO);
		if (saved_stdin == -1)
		{
			error_sys("dup failed\n");
			close(saved_stdout);
			return (1);
		}
		if (cmd->infile)
		{
			if (dup2(cmd->fd_in, STDIN_FILENO) == -1)
			{
				error_sys("dup2 failed\n");
				restore_fds(saved_stdin, saved_stdout);
				return (1);
			}
			close(cmd->fd_in);
		}
		if (cmd->outfile)
		{
			if (dup2(cmd->fd_out, STDOUT_FILENO) == -1)
			{
				error_sys("dup2 failed\n");
				restore_fds(saved_stdin, saved_stdout);
				return (1);
			}
			close(cmd->fd_out);
		}
		if (execute_builtin(sh, cmd, 0))
		{
			restore_fds(saved_stdin, saved_stdout);
			return (1);
		}
		if (restore_fds(saved_stdin, saved_stdout))
			return (1);
		cmd->is_continue = 0;
		return (0);
	}
	pipes->pid[i] = fork();
	if (pipes->pid[i] == -1)
	{
		error_sys("fork failed\n");
		return (1);
	}
	if (pipes->pid[i] == 0)
	{
		reset_signals(sh);
		parse_redirections(sh, cmd, 1);
		child_io(cmd, pipes->fd, i, sh->num_cmds);
		if (is_builtin(cmd))
		{
			if (execute_builtin(sh, cmd, 1))
				exit(1);
			else
				exit(0);
		}
		execute_command(sh, cmd, sh->envp);
		exit(1);
	}
	if (i > 0)
	{
		close(pipes->fd[i - 1][0]);
		close(pipes->fd[i - 1][1]);
	}
	return (error_code);
}

static void	wait_for_children(t_pipes *pipes, t_shell *sh)
{
	int	i;
	int	status;

	i = 0;
	status = 0;
	while (i < sh->num_cmds)
	{
		waitpid(pipes->pid[i], &status, 0);
		if (WIFEXITED(status))
			sh->exit_status = WEXITSTATUS(status);
		i++;
	}
}

void	execute_pipes(t_shell *sh)
{
	int		i;
	t_cmd	*cmd;
	t_pipes	*pipes;
	int	error_code;

	i = 0;
	error_code = 0;
	cmd = NULL;
	pipes = malloc(sizeof(t_pipes));
	if (!pipes)
	{
		error_sys("malloc failed for t_pipes\n");
		sh->exit_status = 1;
		return;
	}
	ft_memset(pipes, 0, sizeof(t_pipes));
	sh->pipes = pipes;
	if (init_pipes(sh->pipes, sh->num_cmds) == 1)
	{
		error_sys("malloc failed for init_pipes\n");
		sh->exit_status = 1;
		return ;
	}
	while (sh->commands[i] != NULL)
	{
		if (init_cmd(&cmd, sh->commands[i], sh) == 1)
		{
			sh->exit_status = 1;
			return ;
		}
		error_code = pipe_and_fork(sh, sh->pipes, i, cmd);
		if (error_code || !cmd->is_continue)
		{
			free_cmd(cmd);
			sh->exit_status = error_code;
			return ;
		}
		free_cmd(cmd);
		i++;
	}
	wait_for_children(sh->pipes, sh);
	return ;
}
