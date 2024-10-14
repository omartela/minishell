/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 15:50:09 by irychkov          #+#    #+#             */
/*   Updated: 2024/10/14 13:41:42 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_array(char ***array)
{
	int i = 0;
	if (!(*array))
		return;
	while ((*array)[i])
	{
		free((*array)[i]);
		(*array)[i] = NULL;
		i++;
	}
	free(*array);
	*array = NULL;
}

void	free_array_back(char **array, size_t i)
{
	if (!array)
		return;
	while (i > 0)
	{
		i--;
		free(array[i]);
		array[i] = NULL;
	}
	free(array);
	array = NULL;
}

void	free_cmd(t_cmd *cmd)
{
	if (!cmd)
		return ;
	if (cmd->args_withquotes)
		free_array(&cmd->args_withquotes);
	if (cmd->args)
		free_array(&cmd->args);
	if (cmd->path)
		free_array(&cmd->path);
	if (cmd->infile)
	{
		free(cmd->infile);
		cmd->infile = NULL;
	}
	if (cmd->outfile)
	{
		free(cmd->outfile);
		cmd->outfile = NULL;
	}
	if (cmd->fd_heredoc)
	{
		free(cmd->fd_heredoc);
		cmd->fd_heredoc = NULL;
	}
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
		sh->pipes->fd[i] = NULL;
		i++;
	}
	free(sh->pipes->fd);
	sh->pipes->fd = NULL;
	free(sh->pipes->pid);
	sh->pipes->pid = NULL;
	free(sh->pipes);
	sh->pipes = NULL;
}

void	free_shell(t_shell *sh)
{
	if (sh->commands)
	{
		free_array(&sh->commands);
		sh->commands = NULL;
	}
	if (sh->homepath)
	{
		free(sh->homepath);
		sh->homepath = NULL;
	}
	if (sh->envp)
		free_array(&sh->envp);
	if (sh->local_shellvars)
		free_array(&sh->local_shellvars);
	if (sh->hd->heredoc_fds)
	{
		free(sh->hd->heredoc_fds);
		sh->hd->heredoc_fds = NULL;
	}
	if (sh->hd)
	{
		free(sh->hd);
		sh->hd = NULL;
	}
	if (sh->pipes)
	{
		free_pipes(sh);
		sh->pipes = NULL;
	}
}

void	free_partial(t_shell *sh)
{
	if (sh->commands)
	{
		free_array(&sh->commands);
		sh->commands = NULL;
	}
	if (sh->hd->heredoc_fds)
	{
		free(sh->hd->heredoc_fds);
		sh->hd->heredoc_fds = NULL;
	}
	if (sh->pipes)
	{
		free_pipes(sh);
		sh->pipes = NULL;
	}
}
