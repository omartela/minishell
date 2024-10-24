/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin-pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 12:36:23 by omartela          #+#    #+#             */
/*   Updated: 2024/10/24 18:46:31 by irychkov         ###   ########.fr       */
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
		ft_putstr_fd("getcwd: cannot access directories: \
No such file or directory\n", 2);
		return (1);
	}
	return (0);
}
