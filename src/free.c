/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 15:50:09 by irychkov          #+#    #+#             */
/*   Updated: 2024/10/11 17:32:18 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_array(char **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void	free_array_back(char **array, size_t i)
{
	while (i > 0)
	{
		i--;
		free(array[i]);
	}
	free(array);
}

void	free_cmd(t_cmd *cmd)
{
/* 	if (!cmd)
		return ; */
	if (cmd->args_withquotes)
		free_array(cmd->args_withquotes);
	if (cmd->args)
		free_array(cmd->args);
	if (cmd->path)
		free_array(cmd->path);
	if (cmd->infile)
		free(cmd->infile);
	if (cmd->outfile)
		free(cmd->outfile);
	if (cmd->fd_heredoc)
		free(cmd->fd_heredoc);
	free(cmd);
	cmd = NULL;
}

void	free_pipes(t_shell *sh)
{
	int	i;

	i = 0;
	while (i < sh->num_cmds - 1)
	{
		free(sh->pipes->fd[i]);
		i++;
	}
	free(sh->pipes->fd);
	free(sh->pipes->pid);
	free(sh->pipes);
	sh->pipes = NULL;
}


void	free_shell(t_shell *sh)
{
	if (sh->commands)
		free_array(sh->commands);
	if (sh->homepath)
		free(sh->homepath);
	if (sh->envp)
		free_array(sh->envp);
	if (sh->local_shellvars)
		free_array(sh->local_shellvars);
	if (sh->hd->heredoc_fds)
		free(sh->hd->heredoc_fds);
	if (sh->hd)
		free(sh->hd);
	if (sh->pipes)
		free_pipes(sh);
}
