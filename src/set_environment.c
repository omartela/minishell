/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_environment.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omartela <omartela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 21:40:31 by omartela          #+#    #+#             */
/*   Updated: 2024/10/22 21:40:53 by omartela         ###   ########.fr       */
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
	char	l_c;
	char	*t_var;

	i = 0;
	len = ft_strlen(var);
	while ((*table)[i])
	{
		t_var = (*table)[i];
		l_c = (*table)[i][len];
		if (ft_strncmp(t_var, var, len + 1) == 0 && !value)
			return (0);
		if (ft_strncmp(t_var, var, len) == 0 && (l_c == '=' || l_c == '\0'))
		{
			if (update_t_var(table, var, value, &i))
				return (1);
			return (0);
		}
		++i;
	}
	if (add_table(table, var, value))
		return (1);
	return (0);
}