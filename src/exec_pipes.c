/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 14:25:13 by irychkov          #+#    #+#             */
/*   Updated: 2024/09/06 16:14:41 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_command(t_cmd *cmd, char **envp)
{
	int		i;
	char	*cmd_path;
	char	*full_path;

	i = 0;
	if (cmd->args[0][0] == '/' || cmd->args[0][0] == '.')
	{
		execve(cmd->args[0], cmd->args, envp); //check if it exists
		free_cmd(cmd);
		perror("execve");
		exit(1);
	}
	if (!cmd->path)
	{
		free_cmd(cmd);
		write(2, "Error: PATH not found\n", 22);
		exit(1);
	}
	while (cmd->path[i])
	{
		cmd_path = ft_strjoin(cmd->path[i], "/");
		if (!cmd_path)
		{
			free_cmd(cmd);
			perror("ft_strjoin");
			exit(1);
		}
		full_path = ft_strjoin(cmd_path, cmd->args[0]);
		free(cmd_path);
		if (!full_path)
		{
			free_cmd(cmd);
			perror("ft_strjoin");
			exit(1);
		}
		execve(full_path, cmd->args, envp);
		free(full_path);
		i++;
	}
	write(2, "Error: command not found\n", 25);
	free_cmd(cmd);
	/* perror("execve failed"); */
	exit(1);
}

int	execute_pipes(t_shell *sh)
{
	int		i;
	t_cmd	*cmd;

	i = 0;
	cmd = NULL;
	while (sh->commands[i] != NULL) {
		i++;
	}
	sh->num_cmds = i;
	int		fd[sh->num_cmds - 1][2];
	pid_t	pid[sh->num_cmds];
	i = 0;
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
			execute_command(cmd, sh->envp);
		}
		free_cmd(cmd);
		i++;
	}
	//parent
	i = 0;
	while (i < sh->num_cmds - 1)
	{
		close(fd[i][0]);
		close(fd[i][1]);
		i++;
	}
	i = 0;
	while (i < sh->num_cmds)
	{
		waitpid(pid[i], NULL, 0);
		i++;
	}
	return (0);
}
