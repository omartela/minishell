/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 15:50:09 by irychkov          #+#    #+#             */
/*   Updated: 2024/10/23 22:45:01 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_array(char ***array)
{
	int	i;

	i = 0;
	if (!(*array))
		return ;
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
		return ;
	while (i > 0)
	{
		i--;
		free(array[i]);
		array[i] = NULL;
	}
	free(array);
	array = NULL;
}

void	close_sh_hd_fds(t_shell *sh, t_cmd *cmd)
{
	int	i;

	i = sh->hd->num_heredocs;
	if (sh->hd->heredoc_fds)
	{
		while (i > 0)
		{
			i--;
			if (sh->hd->heredoc_fds[i] != -1
				&& sh->hd->heredoc_fds[i] != cmd->fd_in)
				close(sh->hd->heredoc_fds[i]);
		}
	}
}

void	free_partial(t_shell *sh)
{
	free_commands_and_promt(sh);
	free_heredoc_fds(sh);
	free_pipes(sh);
}

void	free_shell(t_shell *sh)
{
	free_commands_and_promt(sh);
	free_homepath(sh);
	free_envp_and_shellvars(sh);
	free_heredoc_fds(sh);
	if (sh->hd)
	{
		free(sh->hd);
		sh->hd = NULL;
	}
	free_pipes(sh);
}
