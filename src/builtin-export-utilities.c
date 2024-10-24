/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin-export-utilities.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omartela <omartela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 12:34:12 by omartela          #+#    #+#             */
/*   Updated: 2024/10/24 12:49:12 by omartela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	display_equal_cases(char **l_vars, char *equal, int i)
{
	char	*variable;

	variable = NULL;
	if (equal)
	{
		variable = ft_substr(l_vars[i], 0, (equal - l_vars[i]));
		if (!variable)
			return (1);
	}
	if (equal && *(equal + 1))
		ft_printf("declare -x %s=\"%s\"\n", variable, (equal + 1));
	else if (equal)
		ft_printf("declare -x %s\"\"\n", l_vars[i]);
	else
		ft_printf("declare -x %s\n", l_vars[i]);
	if (variable)
		free(variable);
	return (0);
}

int	display_local_shellvars(t_shell *shell)
{
	int		i;
	char	*variable;
	char	*equal;

	i = 0;
	equal = NULL;
	variable = NULL;
	while (shell->local_shellvars[i])
	{
		if (ft_strncmp("_=", shell->local_shellvars[i], 2) == 0)
		{
			++i;
			continue ;
		}
		equal = ft_strchr(shell->local_shellvars[i], '=');
		if (display_equal_cases(shell->local_shellvars, equal, i))
			return (1);
		++i;
		if (variable)
			free(variable);
		variable = NULL;
	}
	return (0);
}

int	export_add_local(t_shell *sh, char *variable)
{
	if (set_table(&sh->local_shellvars, variable, NULL))
	{
		free(variable);
		return (1);
	}
	else
	{
		free(variable);
		sort_table(sh->local_shellvars);
		return (0);
	}
	free(variable);
	return (0);
}

int	export_add_both(t_shell *sh, char *variable, char *value)
{
	if (set_variables(sh, variable, value))
	{
		free(variable);
		free(value);
		return (1);
	}
	free(variable);
	free(value);
	return (0);
}

int	export_append_both(t_shell *sh, char *variable, char *value)
{
	if (append_table(&sh->envp, variable, value))
	{
		free(variable);
		free(value);
		return (1);
	}
	if (append_table(&sh->local_shellvars, variable, value))
	{
		free(variable);
		free(value);
		return (1);
	}
	sort_table(sh->local_shellvars);
	free(variable);
	free(value);
	return (0);
}
