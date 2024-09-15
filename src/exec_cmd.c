/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 17:58:11 by irychkov          #+#    #+#             */
/*   Updated: 2024/09/15 16:59:30 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	check_permissions(t_cmd *cmd, int is_abs_relative)
{
	int 	fd_test;

	fd_test = -1;
	if (is_abs_relative)
	{
		fd_test = open(cmd->args[0], O_DIRECTORY);
		if (fd_test != -1)
		{
			close(fd_test);
			show_error_free_cmd(126, cmd->args[0], "Is a directory", cmd);
		}
	}
	if ((access(cmd->args[0], F_OK) == -1) && is_abs_relative)
	{
		show_error_free_cmd(127, cmd->args[0], "No such file or directory", cmd);
		exit(127);
	}
	if ((access(cmd->args[0], X_OK) == -1) && is_abs_relative)
	{
		show_error_free_cmd(126, cmd->args[0], "Permission denied", cmd);
		exit(126);
	}
	if (!is_abs_relative)
	{
		show_error_free_cmd(127, cmd->args[0], "command not found", cmd);
		exit(127);
	}
	return ;
}

void	execute_command(t_cmd *cmd, char **envp)
{
	int		i;
	char	*cmd_path;
	char	*full_path;

	i = 0;
	if (cmd->args[0][0] == '/' || cmd->args[0][0] == '.')
	{
		execve(cmd->args[0], cmd->args, envp); //check if it exists
		check_permissions(cmd , 1);
		free_cmd(cmd);
		error_sys("execve failed\n");
		exit(1);
	}
	if (!cmd->path)
		show_error_free_cmd(1, cmd->args[0], "command not found\n", cmd);
	while (cmd->path[i])
	{
		cmd_path = ft_strjoin(cmd->path[i], "/");
		if (!cmd_path)
		{
			free_cmd(cmd);
			error_sys("ft_strjoin failed\n");
			exit(1);
		}
		full_path = ft_strjoin(cmd_path, cmd->args[0]);
		free(cmd_path);
		if (!full_path)
		{
			free_cmd(cmd);
			error_sys("ft_strjoin failed\n");
			exit(1);
		}
		execve(full_path, cmd->args, envp);
		free(full_path);
		i++;
	}
/* 	show_error_free_cmd(127, cmd->args[0], "command not found\n", cmd); */
	/* perror("execve failed"); */
	check_permissions(cmd, 0);
	free_cmd(cmd);
	error_sys("execve failed\n");
	exit(1);
}
