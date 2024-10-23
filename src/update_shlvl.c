/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_shlvl.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omartela <omartela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 12:07:27 by omartela          #+#    #+#             */
/*   Updated: 2024/10/23 12:09:11 by omartela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	update_shlvl_number(t_shell *sh, char *value)
{
	int		temp_atoi;
	char	*temp_itoa;

	temp_atoi = ft_atoi(value);
	free(value);
	if (temp_atoi < 0 || temp_atoi == 0 || temp_atoi >= 2147483647)
	{
		if (set_variables(sh, "SHLVL", "1"))
			return (1);
		return (0);
	}
	temp_atoi += 1;
	temp_itoa = ft_itoa(temp_atoi);
	if (!temp_itoa)
		return (1);
	if (set_variables(sh, "SHLVL", temp_itoa))
	{
		free(temp_itoa);
		return (1);
	}
	free(temp_itoa);
	return (0);
}

static int	update_shlvl_value(t_shell *sh, char *value)
{
	if (is_only_numbers(value))
	{
		if (update_shlvl_number(sh, value))
		{
			error_sys("Setting SHLVL failed\n");
			return (1);
		}
		return (0);
	}
	else
	{
		free(value);
		if (set_variables(sh, "SHLVL", "1"))
		{
			error_sys("Setting SHLVL failed\n");
			return (1);
		}
		return (0);
	}
}

static int	handle_value(t_shell *sh, int i)
{
	char	*value;

	value = get_value(sh->envp[i]);
	if (value)
	{
		if (update_shlvl_value(sh, value))
			return (1);
		return (0);
	}
	else
	{
		error_sys("Allocation failed \n");
		return (1);
	}
}

int	update_shlvl(t_shell *sh)
{
	size_t	i;
	int		keyok;
	int		found;

	i = 0;
	keyok = 0;
	keyok = find_index(&sh->envp, "SHLVL", &found, &i);
	if (keyok == -1)
	{
		error_sys("Allocation failed\n");
		return (1);
	}
	if (found)
	{
		if (handle_value(sh, i))
			return (1);
		return (0);
	}
	if (set_variables(sh, "SHLVL", "1"))
	{
		error_sys("Setting SHLVL failed\n");
		return (1);
	}
	return (0);
}
