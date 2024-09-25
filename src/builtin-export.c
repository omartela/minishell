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
	int	i;
	i = 0;
	while (shell->local_shellvars[i])
	{
		ft_printf("declare -x %s \n", shell->local_shellvars[i]);
		++i;
	}
}

static char	**parse_equal_sign(char *args, char **temp, int *append)
{
	char	*equal;

	equal = ft_strchr(args, '=');
	if (args && equal && (*(equal + 1) != '\0'))
	{
		if (equal && (*(equal - 1) != '-') && (*(equal - 1) != '+'))
		{
			temp = ft_split(args, '=');
			return (temp);
		}
		else if (equal && (*(equal - 1) != '+'))
		{
			*append = 1;
			return (temp);
		}
		return (temp);
	}
	return (temp);
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

int	is_valid_argument_name(const char *name)
{
	int	i;

	i = 0;
	if (!name || !ft_isalpha(name[0]) && name[0] != '_')
		return (0);
	while (name[i])
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
	}
	return (1);
}

static int	is_valid_export_argument(const char *arg)
{

}

int	export(t_shell *shell, char **args)
{
	int		argc;
	char	**temp;
	int 	i;
	int		append;

	argc = 0;
	i = 1;
	append = 0;
	temp = NULL;
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
			temp = parse_equal_sign(args[i], temp, &append);
			if (temp)
			{
				if (set_variables(shell, temp[0], temp[1]))
				{
					free_array(temp);
					return (1);
				}
				free_array(temp);
			}
			else if (!add_table(&shell->local_shellvars, args[i], NULL))
				sort_table(shell->local_shellvars);
			else
				return (1);
			++i;
		}
	}
	return (0);
}
