/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin-export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 14:13:39 by omartela          #+#    #+#             */
/*   Updated: 2024/10/08 17:17:20 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	display_local_shellvars(t_shell *shell)
{
	int		i;
	char	*variable;
	char	*equal;
	i = 0;
	equal = NULL;
	variable = NULL;
	while (shell->local_shellvars[i])
	{
		equal = ft_strchr(shell->local_shellvars[i], '=');
		if (equal)
			variable = ft_substr(shell->local_shellvars[i], 0, (equal - shell->local_shellvars[i]));
		if (equal && *(equal + 1))
			ft_printf("declare -x %s=\"%s\" \n", variable, (equal + 1));
		else if (equal)
			ft_printf("declare -x %s\"\" \n", shell->local_shellvars[i]);
		else
			ft_printf("declare -x %s \n", shell->local_shellvars[i]);
		++i;
		if (variable)
			free(variable);
		variable = NULL;
	}
}

static int	set_variables(t_shell *shell, char *variable, char *value)
{
	int	success1;
	int success2;

	success1 = set_table(&shell->envp, variable, value);
	success2 = set_table(&shell->local_shellvars, variable, value);
	if (!success2)
	{
		sort_table(shell->local_shellvars);
	}
	if (success1 || success2)
		return (1);
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
	if (str[(len - 1)] == '+')
	{
		str[len - 1] = '\0';
		return (1);
	}
	return (0);

}

static int	is_valid_export_argument(const char *arg)
{
	char	*equal_sign;
	char	*variable;
	char	*value;
	int		valid_value;
	int		valid_name;
	int		plus_sign;

	equal_sign = ft_strchr(arg, '=');
	plus_sign = 0;
	if (equal_sign)
	{
		variable = ft_substr(arg, 0, (equal_sign - arg));
		if (!variable)
			return (0);
		value = ft_strdup(equal_sign + 1);
		if (!value)
		{
			free(variable);
			return (1);
		}
		plus_sign = is_last_plus_sign_and_remove(variable);
		valid_value = is_valid_value(value);
		valid_name = is_valid_argument_name(variable);
		if (valid_value && valid_name && !plus_sign)
		{
			free(variable);
			free(value);
			return (1);
		}
		else if (valid_value && valid_name && plus_sign)
		{
			free(variable);
			free(value);
			return (3);
		}
		else
		{
			free(variable);
			free(value);
			return (0);
		}
	}
	else
	{
		if (is_valid_argument_name(arg))
			return (2);
	}
	return (0);
}

static int	parse_export_arg_and_add(t_shell *sh, char *arg)
{
	char	*variable;
	char	*value;
	char	*equal;

	if (is_valid_export_argument(arg) == 1)
	{
		equal = ft_strchr(arg, '=');
		if (!equal)
			return (0);
		variable = ft_substr(arg, 0, (equal - arg));
		if (!variable)
			return (0);
		value = ft_strdup(equal + 1);
		if (!value)
		{
			free(variable);
			return (1);
		}
		if (set_variables(sh, variable, value))
		{
			free(variable);
			free(value);
			return (1);
		}
		return (0);
	}
	else if (is_valid_export_argument(arg) == 2)
	{
		if (set_table(&sh->local_shellvars, arg, NULL))
			return (1);
		else
		{
			sort_table(sh->local_shellvars);
			return (0);
		}
	}
	else if (is_valid_export_argument(arg) == 3)
	{
		equal = ft_strchr(arg, '=');
		if (!equal)
			return (0);
		variable = ft_substr(arg, 0, (equal - arg));
		if (!variable)
			return (0);
		value = ft_strdup(equal + 1);
		if (!value)
		{
			free(variable);
			return (1);
		}
		is_last_plus_sign_and_remove(variable);
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
	return (1);
}

int	export(t_shell *shell, char **args)
{
	int		argc;
	int 	i;

	argc = 0;
	i = 1;
	while (args[argc])
		argc++;
	if (argc == 1)
	{
		display_local_shellvars(shell);
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
