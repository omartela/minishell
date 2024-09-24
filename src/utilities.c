/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omartela <omartela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 11:06:00 by omartela          #+#    #+#             */
/*   Updated: 2024/09/10 17:14:46 by omartela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/* void	execute_builtin_command(t_cmd *cmd, t_shell *shell)
{
	return ;
} */

char	*expand_tilde(t_shell *sh)
{
	return (sh->homepath);
}

int	is_builtin(t_cmd *cmd)
{
	char **str;
	int i;
	int is_builtin;

	str = ft_split("export,cd,unset,pwd,echo", ',');
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