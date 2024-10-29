/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 16:05:06 by irychkov          #+#    #+#             */
/*   Updated: 2024/10/28 17:46:19 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_num_cmds(t_shell *sh)
{
	int	i;

	i = 0;
	while (sh->commands[i] != NULL)
		i++;
	sh->num_cmds = i;
}

static int	initialize_shlvl(t_shell *sh)
{
	if (update_shlvl(sh))
	{
		free_envp_and_shellvars(sh);
		return (1);
	}
	return (0);
}

static int	initialize_homepath(t_shell *sh, char **envp)
{
	sh->homepath = expand(envp, "HOME");
	if (!sh->homepath)
	{
		error_sys("Expand home failed\n");
		free_envp_and_shellvars(sh);
		return (1);
	}
	return (0);
}

static int	initialize_heredoc(t_shell *sh)
{
	sh->hd = ft_calloc(1, sizeof(t_heredoc));
	if (!sh->hd)
	{
		error_sys("t_heredoc failed\n");
		free_homepath(sh);
		free_envp_and_shellvars(sh);
		return (1);
	}
	return (0);
}

void	initialize_shell(t_shell *sh, char ***envp)
{
	ft_memset(sh, 0, sizeof(t_shell));
	initialize_env(sh, envp);
	if (initialize_shlvl(sh) || initialize_homepath(sh, sh->envp)
		|| (initialize_heredoc(sh)))
		exit(1);
	if (init_signal(sh))
	{
		error_sys("Error when initializing signal\n");
		free_shell(sh);
		exit (1);
	}
}
