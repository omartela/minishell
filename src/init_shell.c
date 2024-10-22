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


static int increase_shlvl(t_shell *sh)
{
	char	*temp_itoa;
	int		temp_atoi;
	char	*value;
	int		i;
	int		j;
	int		keyok;

	i = 0;
	j = 0;
	keyok = 0;
	temp_atoi = 0;
	temp_itoa = NULL;
	value = NULL;

	while (sh->envp[i])
	{
		keyok = is_check_key_equal(sh->envp[i], "SHLVL");
		if (keyok == 1)
		{
			value = get_value(sh->envp[i]);
			if (value)
			{
				j = 0;
				while (value[j])
				{
					if (!ft_isdigit(value[j]))
					{
						free(value);
						if (set_variables(sh, "SHLVL", "1"))
						{
							error_sys("Setting SHLVL failed\n");
							return (1);
						}
						return (0);
					}
					j++;
				}
				temp_atoi = ft_atoi(value);
				free(value);
				if (temp_atoi < 0 || temp_atoi == 0 || temp_atoi == 2147483647)
				{
					if (set_variables(sh, "SHLVL", "1"))
					{
						error_sys("Setting SHLVL failed\n");
						return (1);
					}
					return (0);
				}
				temp_atoi += 1;
				temp_itoa = ft_itoa(temp_atoi);
				if (!temp_itoa)
				{
					error_sys("Setting SHLVL failed\n");
					return (1);
				}
				if (set_variables(sh, "SHLVL", temp_itoa))
				{
					free(temp_itoa);
					error_sys("Setting SHLVL failed\n");
					return (1);
				}
				free(temp_itoa);
				return (0);
			}
			else
			{
					/// allocating failed need to return properly
					error_sys("Setting SHLVL failed\n");
					return (1);
			}
		}
		else if (keyok == -1)
		{
			/// allocating failed need to return properly
			return (1);
		}
		++i;
	}
	if (append_table(&sh->envp, "SHLVL", "1"))
	{
		error_sys("Setting SHLVL failed\n");
		return (1);
	}
	if (append_table(&sh->local_shellvars, "SHLVL", "1"))
	{
		error_sys("Setting SHLVL failed\n");
		return (1);
	}
	sort_table(sh->local_shellvars);
	return (0);
}

void	initialize_shell(t_shell *sh, char ***envp)
{
	t_heredoc	*hd;

	ft_memset(sh, 0, sizeof(t_shell));
	copy_env(*envp, sh);
	*envp = sh->envp;
	if (increase_shlvl(sh))
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