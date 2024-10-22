/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 17:58:11 by irychkov          #+#    #+#             */
/*   Updated: 2024/10/22 10:47:43 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_full_command_path(char *path, char *command, t_cmd *cmd)
{
	char	*cmd_path;
	char	*full_path;

	cmd_path = ft_strjoin(path, "/");
	if (!cmd_path)
	{
		free_cmd(&cmd);
		error_sys("ft_strjoin failed\n");
		exit(1);
	}
	full_path = ft_strjoin(cmd_path, command);
	free(cmd_path);
	if (!full_path)
	{
		free_cmd(&cmd);
		error_sys("ft_strjoin failed\n");
		exit(1);
	}
	return (full_path);
}

static void	check_permissions(t_shell *sh, t_cmd *cmd, int is_abs_relative)
{
	int	fd_test;

	fd_test = -1;
	if (is_abs_relative)
	{
		fd_test = open(cmd->args[0], O_DIRECTORY);
		if (fd_test != -1)
		{
			close(fd_test);
			show_error(cmd->args[0], "Is a directory");
			exit_and_free(sh, cmd, 126);
		}
	}
	if ((access(cmd->args[0], F_OK) == -1) && is_abs_relative)
	{
		show_error(cmd->args[0], "No such file or directory");
		exit_and_free(sh, cmd, 127);
	}
	if ((access(cmd->args[0], X_OK) == -1) && is_abs_relative)
	{
		show_error(cmd->args[0], "Permission denied");
		exit_and_free(sh, cmd, 126);
	}
	if (!is_abs_relative)
	{
		show_error(cmd->args[0], "command not found");
		exit_and_free(sh, cmd, 127);
	}
	return ;
}

static void	execute_absolute_relative_command(t_shell *sh, t_cmd *cmd, char **envp)
{
	if (ft_strncmp(cmd->args[0], ".\0", 2) == 0)
	{
		show_error(cmd->args[0], "filename argument required");
		exit_and_free(sh, cmd, 2);
	}
	if (ft_strncmp(cmd->args[0], "..\0", 3) == 0)
	{
		show_error(cmd->args[0], "command not found");
		exit_and_free(sh, cmd, 127);
	}
	execve(cmd->args[0], cmd->args, envp);
	check_permissions(sh, cmd, 1);
	error_sys("execve failed\n");
	exit_and_free(sh, cmd, 1);
}

void	execute_command(t_shell *sh, t_cmd *cmd, char **envp)
{
	int		i;
	char	*full_path;

	i = 0;
	full_path = NULL;
	if (cmd->args[0] == NULL)
		exit_and_free(sh, cmd, 0);
	if (cmd->args[0][0] == '/' || cmd->args[0][0] == '.')
		execute_absolute_relative_command(sh, cmd, envp);
	if (!cmd->path)
	{
		execve(cmd->args[0], cmd->args, envp);
		check_permissions(sh, cmd, 1);
		error_sys("execve failed\n");
		exit_and_free(sh, cmd, 1);
	}
	while (cmd->path[i])
	{
		full_path = get_full_command_path(cmd->path[i], cmd->args[0], cmd);
		execve(full_path, cmd->args, envp);
		free(full_path);
		i++;
	}
	check_permissions(sh, cmd, 0);
	error_sys("execve failed\n");
	exit_and_free(sh, cmd, 1);
}
