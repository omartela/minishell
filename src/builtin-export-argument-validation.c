/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin-export-argument-validation.c               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omartela <omartela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 12:37:27 by omartela          #+#    #+#             */
/*   Updated: 2024/10/24 12:37:41 by omartela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

int	is_valid_export_argument(char *variable, char *value, char *equal)
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
