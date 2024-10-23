/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 14:25:13 by irychkov          #+#    #+#             */
/*   Updated: 2024/10/23 16:34:00 by irychkov         ###   ########.fr       */
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

static int	pipe_and_fork(t_shell *sh, t_cmd *cmd, int i)
{
	int	error_code;
	int is_build;

	error_code = 0;
	if (i < sh->num_cmds - 1)
	{
		if (pipe(sh->pipes->fd[i]) == -1)
		{
			error_sys("pipe failed\n");
			return (1);
		}
	}
	is_build = is_builtin(cmd);
	if (sh->num_cmds == 1 && is_build == 1)
	{
		error_code = parse_redirections(sh ,cmd, 0);
		if (error_code)
			return (error_code);
		cmd->saved_std[1] = dup(STDOUT_FILENO); /// Make a function to protect all dups or something...
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
	else if(is_build == -1)
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
	if (i > 0)
	{
		close(sh->pipes->fd[i - 1][0]);
		close(sh->pipes->fd[i - 1][1]);
	}
	return (error_code);
}

static void	wait_for_children(t_pipes *pipes, t_shell *sh)
{
	int	i;
	int	status;
	int signal_number;

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

void	execute_pipes(t_shell *sh)
{
	int		i;
	t_cmd	*cmd;
	t_pipes	*pipes;
	int	error_code;

	i = 0;
	error_code = 0;
	cmd = NULL;
	pipes = ft_calloc(1, sizeof(t_pipes));
	if (!pipes)
	{
		error_sys("malloc failed for t_pipes\n");
		sh->exit_status = 1;
		return;
	}
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
		if (update_underscore(sh, cmd))
		{
			sh->exit_status = 1;
			return ;
		}
		error_code = pipe_and_fork(sh, cmd, i);
		if (error_code || !cmd->is_continue)
		{
			free_cmd(&cmd);
			sh->exit_status = error_code;
			return ;
		}
		free_cmd(&cmd);
		i++;
	}
	wait_for_children(sh->pipes, sh);
	return ;
}
