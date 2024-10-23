/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin-pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omartela <omartela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 12:36:23 by omartela          #+#    #+#             */
/*   Updated: 2024/10/23 10:51:06 by omartela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../include/minishell.h"

int	pwd(void)
{
	char	cwd[PATH_MAX];
	char	*str;

	str = getcwd(cwd, sizeof(cwd));
	if (str != NULL)
	{
		ft_putstr_fd(str, 1);
		write(1, "\n", 1);
		return (0);
	}
	else
	{
		ft_putstr_fd("getcwd: cannot access directories: No such file or directory\n", 2);
		return (1);
	}
	return (0);
}
