/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 17:58:11 by irychkov          #+#    #+#             */
/*   Updated: 2024/09/10 12:07:34 by irychkov         ###   ########.fr       */
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
	show_error_free_cmd(1, cmd->args[0], "command not found\n", cmd);
	/* perror("execve failed"); */
}
