/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 11:06:00 by omartela          #+#    #+#             */
/*   Updated: 2024/10/14 15:41:17 by irychkov         ###   ########.fr       */
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

	split= ft_split(args, '=');
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
	char **str;
	int i;
	int is_builtin;

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
		if (cmd->args[0] && ft_strncmp(cmd->args[0], str[i], ft_strlen(str[i]) + 1) == 0)
			is_builtin = 1;
		i++;
	}
	i = 0;
	free_array(&str);
	return (is_builtin);
}

int	execute_builtin(t_shell *sh, t_cmd *cmd, int is_in_pipe)
{
	if (ft_strncmp(cmd->args[0], "exit\0", 5) == 0)
	{
		if (!is_in_pipe)
			return (exit_shell(sh, cmd));
		else
			exit(exit_shell(sh, cmd));
	}
	if (ft_strncmp(cmd->args[0], "export\0", 7) == 0)
	{
		if (!is_in_pipe)
		{
			if (export(sh, cmd->args))
			{
				sh->exit_status = 1;
				return (1);
			}
			sh->exit_status = 0;
			return (0);
		}
		else
		{
			if (export(sh, cmd->args))
				exit_and_free(sh, cmd, 1);
			exit_and_free(sh, cmd, 0);
		}
	}
	if (ft_strncmp(cmd->args[0], "env\0", 4) == 0)
	{
		if (!is_in_pipe)
		{
			if (env(sh, cmd->args))
			{
				sh->exit_status = 1;
				return (1);
			}
			sh->exit_status = 0;
			return (0);
		}
		else
		{
			if (env(sh, cmd->args))
				exit_and_free(sh, cmd, 1);
			exit_and_free(sh, cmd, 0);
		}
	}
	if (ft_strncmp(cmd->args[0], "cd\0", 3) == 0)
	{
		if (!is_in_pipe)
		{
			if (cd(sh, cmd->args))
			{
				sh->exit_status = 1;
				return (1);
			}
			sh->exit_status = 0;
			return (0);
		}
		else
		{
			if (cd(sh, cmd->args))
				exit_and_free(sh, cmd, 1);
			exit_and_free(sh, cmd, 0);
		}
	}
	if (ft_strncmp(cmd->args[0], "unset\0", 6) == 0)
	{
		if (!is_in_pipe)
		{
			if (unset(sh, cmd->args))
			{
				sh->exit_status = 1;
				return (1);
			}
			sh->exit_status = 0;
			return (0);
		}
		else
		{
			if (unset(sh, cmd->args))
				exit_and_free(sh, cmd, 1);
			exit_and_free(sh, cmd, 0);
		}
	}
	if (ft_strncmp(cmd->args[0], "pwd\0", 4) == 0)
	{
		if (!is_in_pipe)
		{
			if (pwd())
			{
				sh->exit_status = 1;
				return (1);
			}
			sh->exit_status = 0;
			return (0);
		}
		else
		{
			if (pwd())
				exit_and_free(sh, cmd, 1);
			exit_and_free(sh, cmd, 0);
		}
	}
	if (ft_strncmp(cmd->args[0], "echo\0", 5) == 0)
	{
		if (!is_in_pipe)
		{	
			if (echo(cmd->args))
			{
				sh->exit_status = 1;
				return (1);
			}
			sh->exit_status = 0;
			return (0);
		}
		else
		{
			if (echo(cmd->args))
				exit_and_free(sh, cmd, 1);
			exit_and_free(sh, cmd, 0);
		}

	}
	return (1);
}
