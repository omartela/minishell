/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin-export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omartela <omartela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 14:13:39 by omartela          #+#    #+#             */
/*   Updated: 2024/09/11 11:59:47 by omartela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	display_local_shellvars(t_shell *shell)
{
	int		i;
	char	**temp;
	i = 0;
	while (shell->local_shellvars[i])
	{
		temp = ft_split(shell->local_shellvars[i], '=');
		if (temp[1])
			ft_printf("declare -x %s=\"%s\" \n", temp[0], temp[1]);
		else if (ft_strchr(shell->local_shellvars[i], '='))
			ft_printf("declare -x %s\"\" \n", shell->local_shellvars[i]);
		else
			ft_printf("declare -x %s \n", shell->local_shellvars[i]);
		++i;
		free_array(temp);
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
	if ((!name || !ft_isalpha(name[0])) && name[0] != '_')
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

static int	is_valid_export_argument(const char *arg)
{
	char	*equal_sign;
	char	**variable_value;
	int		valid_value;
	int		valid_name;

	equal_sign = ft_strchr(arg, '=');
	if (equal_sign)
	{
		variable_value = ft_split(arg, '=');
		if (!variable_value)
			return (0);
		valid_value = is_valid_value(variable_value[1]);
		valid_name = is_valid_argument_name(variable_value[0]);
		//free_array(variable_value);
		if (valid_value && valid_name)
			return (1);
		else
			return (0);
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
	char	**var_value;

	if (is_valid_export_argument(arg) == 1)
	{
		var_value = ft_split(arg, '=');
		if (var_value[1] == NULL)
		{
			set_variables(sh, var_value[0], "");
			return (0);
		}
		if (set_variables(sh, var_value[0], var_value[1]))
		{
			free_array(var_value);
			return (1);
		}
		free_array(var_value);
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
	return (1);
}

int	export(t_shell *shell, char **args)
{
	int		argc;
	int 	i;
	int		append;

	argc = 0;
	i = 1;
	append = 0;
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
				return (1);
			++i;
		}
	}
	return (0);
}
