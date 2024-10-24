/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 11:06:00 by omartela          #+#    #+#             */
/*   Updated: 2024/10/23 15:50:33 by omartela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_only_numbers(char *str)
{
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (0);
		++str;
	}
	return (1);
}

char	*get_value(char *args)
{
	char	*value;
	char	*equal;

	value = NULL;
	equal = ft_strchr(args, '=');
	if (equal)
	{
		value = ft_strdup((equal + 1));
		if (!value)
			return (NULL);
	}
	else
	{
		value = ft_strdup("");
		if (!value)
			return (NULL);
	}
	return (value);
}

char	*get_key(char *args)
{
	char	**split;
	char	*key;

	split = ft_split(args, '=');
	if (!split)
		return (NULL);
	key = ft_strdup(split[0]);
	if (!key)
	{
		free_array(&split);
		return (NULL);
	}
	free_array(&split);
	return (key);
}

int	is_check_key_equal(char *args, const char *variable)
{
	char	*key;
	size_t	len;

	key = get_key(args);
	if (!key)
		return (-1);
	len = ft_strlen(key);
	if (ft_strncmp(key, variable, len + 1) == 0)
	{
		free(key);
		return (1);
	}
	free(key);
	return (0);
}

int	is_builtin(t_cmd *cmd)
{
	char	**str;
	int		i;
	int		is_builtin;

	str = ft_split("exit,export,cd,env,unset,pwd,echo", ',');
	i = 0;
	is_builtin = 0;
	if (!str)
	{
		error_sys("is_builtin failed\n");
		return (-1);
	}
	while (str[i])
	{
		if (cmd->args[0] && \
		ft_strncmp(cmd->args[0], str[i], ft_strlen(str[i]) + 1) == 0)
			is_builtin = 1;
		i++;
	}
	i = 0;
	free_array(&str);
	return (is_builtin);
}
