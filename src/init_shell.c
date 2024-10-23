/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_shell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 15:29:41 by irychkov          #+#    #+#             */
/*   Updated: 2024/10/22 15:30:03 by irychkov         ###   ########.fr       */
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

void	initialize_shell(t_shell *sh, char ***envp)
{
	t_heredoc	*hd;

	ft_memset(sh, 0, sizeof(t_shell));
	copy_env(*envp, sh);
	*envp = sh->envp;
	if (update_shlvl(sh))
	{
		free_array(&sh->envp);
		free_array(&sh->local_shellvars);
		exit (1);
	}
	sh->homepath = expand(*envp, "HOME");
	if (!sh->homepath)
	{
		error_sys("Expand home failed\n");
		free_array(&sh->envp);
		free_array(&sh->local_shellvars);
		exit (1);
	}
	hd = ft_calloc(1, sizeof(t_heredoc));
	if (!hd)
	{
		error_sys("t_heredoc failed\n");
		free(sh->homepath);
		free_array(&sh->envp);
		free_array(&sh->local_shellvars);
		exit(1);
	}
	sh->hd = hd;
	ft_memset(&sh->org_sig_int, 0, sizeof(sh->org_sig_int));
	ft_memset(&sh->org_sig_quit, 0, sizeof(sh->org_sig_quit));
	if (init_signal(sh))
	{
		error_sys("Error when initializing signal\n");
		free_shell(sh);
		exit (1);
	}
}
