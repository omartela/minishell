/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 22:00:30 by irychkov          #+#    #+#             */
/*   Updated: 2024/10/23 22:17:33 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_commands_and_promt(t_shell *sh)
{
	if (sh->commands)
	{
		free_array(&sh->commands);
		sh->commands = NULL;
	}
	if (sh->promt)
	{
		free(sh->promt);
		sh->promt = NULL;
	}
}

void	free_homepath(t_shell *sh)
{
	if (sh->homepath)
	{
		free(sh->homepath);
		sh->homepath = NULL;
	}
}

void	free_envp_and_shellvars(t_shell *sh)
{
	if (sh->envp)
	{
		free_array(&sh->envp);
	}
	if (sh->local_shellvars)
	{
		free_array(&sh->local_shellvars);
	}
}

void	free_heredoc_fds(t_shell *sh)
{
	if (sh->hd && sh->hd->heredoc_fds)
	{
		while (sh->hd->num_heredocs > 0)
		{
			sh->hd->num_heredocs--;
			if (sh->hd->heredoc_fds[sh->hd->num_heredocs] != -1)
				close(sh->hd->heredoc_fds[sh->hd->num_heredocs]);
		}
		free(sh->hd->heredoc_fds);
		sh->hd->heredoc_fds = NULL;
	}
}

void	free_pipes(t_shell *sh)
{
	int	i;

	i = 0;
	if (!sh->pipes)
		return ;
	while (i < sh->num_cmds - 1)
	{
		if (sh->pipes->fd[i][0] != -1)
			close(sh->pipes->fd[i][0]);
		if (sh->pipes->fd[i][1] != -1)
			close(sh->pipes->fd[i][1]);
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
