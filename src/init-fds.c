/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init-fds.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 14:35:09 by irychkov          #+#    #+#             */
/*   Updated: 2024/09/05 15:42:22 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	init_fdin(t_cmd *cmd)
{
	if (cmd->infile)
	{
		if (access(cmd->infile, F_OK) == -1)
		{
			perror("No such file or directory");
			exit(127);
		}
		if (access(cmd->infile, R_OK) == -1)
		{
			perror("Permission denied");
			exit(126);
		}
		cmd->fd_in = open(cmd->infile, O_RDONLY);
		if (cmd->fd_in == -1)
		{
			perror("open");
			exit(1);
		}
	}
	else
		cmd->fd_in = STDIN_FILENO;
}

static void	init_fdout(t_cmd *cmd)
{
	if (cmd->outfile)
	{
		cmd->fd_out = open(cmd->outfile, O_DIRECTORY);
		if (cmd->fd_out != -1)
		{
			perror("Is a directory");
			exit(126);
		}
		if (access(cmd->outfile, F_OK) == 0 && access(cmd->outfile, W_OK) == -1)
		{
			perror("Permission denied");
			exit(1);
		}
		if (cmd->append)
		{
			cmd->fd_out = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (cmd->fd_out == -1)
			{
				perror("open");
				exit(1);
			}
		}
		else
		{
			cmd->fd_out = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (cmd->fd_out == -1)
			{
				perror("open");
				exit(1);
			}
		}
	}
	else
		cmd->fd_out = STDOUT_FILENO;
}

void	init_fds(t_cmd *cmd)
{
	init_fdin(cmd);
	init_fdout(cmd);
}
