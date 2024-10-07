/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 11:06:00 by omartela          #+#    #+#             */
/*   Updated: 2024/10/02 22:00:38 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*get_value(char *args)
{
	char	**split;
	char	*value;

	split = ft_split(args, '=');
	if (!split)
		return (NULL);
	value = ft_strdup(split[1]);
	free_array(split);
	return (value);
}

char	*get_key(char *args)
{
	char	**split;
	char	*key;

	split= ft_split(args, '=');
	if (!split)
		return (NULL);
	key = ft_strdup(split[0]);
	free_array(split);
	return (key);
}

int	is_check_key_equal(char *args, const char *variable)
{
	char	*key;
	size_t	len;

	key = get_key(args);
	if (!key)
		return (0);
	len = ft_strlen(key);
	if (ft_strncmp(key, variable, len + 1) == 0)
	{
		free(key);
		return (1);
	}
	free(key);
	return (0);
}


char	*expand_tilde(t_shell *sh)
{
	return (sh->homepath);
}

int	is_builtin(t_cmd *cmd)
{
	char **str;
	int i;
	int is_builtin;

	str = ft_split("exit,export,cd,env,unset,pwd,echo", ',');
	i = 0;
	is_builtin = 0;
	while (str[i])
	{
		if (ft_strncmp(cmd->args[0], str[i], ft_strlen(str[i]) + 1) == 0)
			is_builtin = 1;
		i++;
	}
	i = 0;
	while (str[i])
	{
		free(str[i]);
		++i;
	}
	free(str);
	return (is_builtin);
}

int	execute_builtin(t_shell *sh, t_cmd *cmd)
{
	if (ft_strncmp(cmd->args[0], "exit\0", 5) == 0)
		return (exit_shell(sh, cmd->args));
	if (ft_strncmp(cmd->args[0], "export\0", 7) == 0)
	{
		if (export(sh, cmd->args))
		{
			sh->exit_status = 1;
			return (1);
		}
		sh->exit_status = 0;
		return (0);
	}
	if (ft_strncmp(cmd->args[0], "env\0", 4) == 0)
	{
		if (env(sh, cmd->args))
		{
			sh->exit_status = 1;
			return (1);
		}
		sh->exit_status = 0;
		return (0);
	}
	if (ft_strncmp(cmd->args[0], "cd\0", 3) == 0)
	{
		if (cd(sh, cmd->args))
		{
			sh->exit_status = 1;
			return (1);
		}
		sh->exit_status = 0;
		return (0);
	}
	if (ft_strncmp(cmd->args[0], "unset\0", 6) == 0)
	{
		if (unset(sh, cmd->args))
		{
			sh->exit_status = 1;
			return (1);
		}
		sh->exit_status = 0;
		return (0);
	}
	if (ft_strncmp(cmd->args[0], "pwd\0", 4) == 0)
	{
		if (pwd())
		{
			sh->exit_status = 1;
			return (1);
		}
		sh->exit_status = 0;
		return (0);
	}
	if (ft_strncmp(cmd->args[0], "echo\0", 5) == 0)
	{
		if (echo(cmd->args))
		{
			sh->exit_status = 1;
			return (1);
		}
		sh->exit_status = 0;
		return (0);
	}
	return (1);
}
