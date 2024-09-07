/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 15:50:09 by irychkov          #+#    #+#             */
/*   Updated: 2024/09/07 23:20:47 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	if (cmd->args)
		free_array(cmd->args);
	if (cmd->path)
		free_array(cmd->path);
	free(cmd);
}

void	free_shell(t_shell *sh)
{
	if (sh->commands)
		free_array(sh->commands);
	if (sh->homepath)
		free(sh->homepath);
/* 	if (sh->envp)
		free_array(sh->envp); */
}

void	free_pipes(t_pipes *pipes, int num_cmds)
{
	int	i;

	i = 0;
	while (i < num_cmds - 1)
	{
		free(pipes->fd[i]);
		i++;
	}
	free(pipes->fd);
	free(pipes->pid);
}
