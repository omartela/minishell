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

static int	display_local_shellvars(t_shell *shell)
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
		if (equal)
		{
			variable = ft_substr(shell->local_shellvars[i], 0, (equal - shell->local_shellvars[i]));
			if (!variable)
				return (1);
		}
		if (equal && *(equal + 1))
			ft_printf("declare -x %s=\"%s\"\n", variable, (equal + 1));
		else if (equal)
			ft_printf("declare -x %s\"\"\n", shell->local_shellvars[i]);
		else
			ft_printf("declare -x %s\n", shell->local_shellvars[i]);
		++i;
		if (variable)
			free(variable);
		variable = NULL;
	}
	return (0);
}

static int	is_valid_argument_name(const char *name)
{
	int	i;

	i = 0;
	if (!name)
		return (0);
	if (!ft_isalpha(name[0]) && name[0] != '_')
		return (0);
	while (name[i])
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
		++i;
	}
	return (1);
}

static int	is_valid_value(const char *value)
{
	if (value)
		return (1);
	return (1);
}

static int	is_last_plus_sign_and_remove(char *str)
{
	int	len;

	if (!str)
		return (0);
	len = ft_strlen(str);
	if (len > 0 && str[(len - 1)] == '+')
	{
		str[len - 1] = '\0';
		return (1);
	}
	return (0);
}

static int	is_valid_export_argument(char *variable, char *value, char *equal)
{
	int		valid_value;
	int		valid_name;
	int		plus_sign;

	plus_sign = 0;
	if (equal)
	{
		plus_sign = is_last_plus_sign_and_remove(variable);
		valid_value = is_valid_value(value);
		valid_name = is_valid_argument_name(variable);
		if (valid_value && valid_name && !plus_sign)
			return (1);
		else if (valid_value && valid_name && plus_sign)
			return (3);
		else
			return (0);
	}
	else
	{
		if (is_valid_argument_name(variable))
			return (2);
	}
	return (0);
}

static int	export_add_local(t_shell *sh, char *variable)
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

static int	export_add_both(t_shell *sh, char *variable, char *value)
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

static int	export_append_both(t_shell *sh, char *variable, char *value)
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

static int	parse_export_argument(char **arg, char **variable, char **value, char **equal)
{
	*equal = ft_strchr(*arg, '=');
	if (*equal)
	{
		*variable = ft_substr(*arg, 0, (*equal - *arg));
		if (!(*variable))
			return (1);
		*value = ft_strdup(*equal + 1);
		if (!(*value))
		{
			free(*variable);
			return (1);
		}
	}
	else
	{
		*variable = ft_strdup(*arg);
		if (!(*variable))
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
	if (parse_export_argument(&arg, &variable, &value, &equal))
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
				return (show_error_return(1, args[i], "not a valid identifier"));
			++i;
		}
	}
	return (0);
}
 
