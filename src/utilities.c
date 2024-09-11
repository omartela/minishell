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

void	execute_builtin_command(t_cmd *cmd, t_shell *shell)
{
	if (ft_strncmp(cmd->args[0], "export", ft_strlen(cmd->args[0])) == 0)
	{
		if (export(shell, cmd->args))
			exit(1);
		else
			exit(0);
	}
}

char	*expand_tilde(t_shell *sh)
{
	return (sh->homepath);
}
