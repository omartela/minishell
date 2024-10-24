/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin-export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 14:13:39 by omartela          #+#    #+#             */
/*   Updated: 2024/10/23 13:22:37 by omartela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	parse_export_arg(char **arg, char **var, char **value, char **equal)
{
	*equal = ft_strchr(*arg, '=');
	if (*equal)
	{
		*var = ft_substr(*arg, 0, (*equal - *arg));
		if (!(*var))
			return (1);
		*value = ft_strdup(*equal + 1);
		if (!(*value))
		{
			free(*var);
			return (1);
		}
	}
	else
	{
		*var = ft_strdup(*arg);
		if (!(*var))
			return (1);
	}
	return (0);
}

static int	add_arg(t_shell *sh, char *variable, char *value, int result)
{
	if (result == 1)
	{
		if (export_add_both(sh, variable, value))
			return (1);
		return (0);
	}
	else if (result == 2)
	{
		free(value);
		if (export_add_local(sh, variable))
			return (1);
		return (0);
	}
	else if (result == 3)
	{
		if (export_append_both(sh, variable, value))
			return (1);
		return (0);
	}
	free(variable);
	free(value);
	return (1);
}

static int	parse_export_arg_and_add(t_shell *sh, char *arg)
{
	char	*variable;
	char	*value;
	char	*equal;
	int		result;

	variable = NULL;
	value = NULL;
	equal = NULL;
	if (parse_export_arg(&arg, &variable, &value, &equal))
		return (1);
	result = is_valid_export_argument(variable, value, equal);
	if (add_arg(sh, variable, value, result))
		return (1);
	return (0);
}

int	export(t_shell *shell, char **args)
{
	int		argc;
	int		i;

	argc = 0;
	i = 1;
	while (args[argc])
		argc++;
	if (argc == 1)
	{
		if (display_local_shellvars(shell))
			return (1);
		return (0);
	}
	if (argc >= 2)
	{
		while (i < argc)
		{
			if (parse_export_arg_and_add(shell, args[i]))
				return (show_error_return(1, args[i], "not a \
			valid identifier"));
			++i;
		}
	}
	return (0);
}
