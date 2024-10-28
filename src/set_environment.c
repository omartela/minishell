/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_environment.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 21:40:31 by omartela          #+#    #+#             */
/*   Updated: 2024/10/28 12:28:01 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	set_variables(t_shell *shell, char *variable, char *value)
{
	int	success1;
	int	success2;

	success1 = set_table(&shell->envp, variable, value);
	success2 = set_table(&shell->local_shellvars, variable, value);
	if (success1 || success2)
		return (1);
	if (success2 == 0)
	{
		sort_table(shell->local_shellvars);
	}
	return (0);
}

int	set_table(char ***table, const char *var, const char *value)
{
	size_t	i;
	size_t	len;

	i = 0;
	len = ft_strlen(var);
	while ((*table)[i])
	{
		if (len <= ft_strlen((*table)[i]))
		{
			if (!value && ft_strncmp((*table)[i], var, len + 1) == 0)
				return (0);
			if (ft_strncmp((*table)[i], var, len) == 0
				&& ((*table)[i][len] == '=' || (*table)[i][len] == '\0'))
			{
				if (update_t_var(table, var, value, &i))
					return (1);
				return (0);
			}
		}
		++i;
	}
	if (add_table(table, var, value))
		return (1);
	return (0);
}
