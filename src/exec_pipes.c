/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 14:25:13 by irychkov          #+#    #+#             */
/*   Updated: 2024/09/04 22:05:27 by irychkov         ###   ########.fr       */
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
	int		fd[2];
	int		prev_fd;
	pid_t	pid;
	t_cmd	*cmd;

	i = 0;
	cmd = NULL;
	prev_fd = -1;
	ft_memset(fd, -1, sizeof(fd));
	while (sh->commands[i] != NULL)
	{
		if (init_cmd(&cmd, sh->commands[i], sh->envp) == 1)
			return (1);
		if (sh->commands[i + 1] != NULL)
		{
			if (pipe(fd) == -1)
			{
				free_cmd(cmd);
				perror("pipe");
				return (1);
			}
		}
		pid = fork();
		if (pid == -1)
		{
			free_cmd(cmd);
			perror("fork");
			return (1);
		}
		if (pid == 0)
		{//child
			if (prev_fd != -1)
			{
				dup2(prev_fd, STDIN_FILENO);
				close(prev_fd);
			}
			if (sh->commands[i + 1] != NULL)
			{
				close(fd[0]);
				dup2(fd[1], STDOUT_FILENO);
				close(fd[1]);
			}
			execute_command(cmd, sh->envp);
		}
		else
		{//parent
			waitpid(pid, NULL, 0);
			if (prev_fd != -1)
			{
				close(prev_fd);
			}
			if (sh->commands[i + 1] != NULL)
			{
				close(fd[1]);
				prev_fd = fd[0];
			}
			free_cmd(cmd);
		}
		i++;
	}
	return (0);
}
