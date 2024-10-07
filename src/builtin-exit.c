/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin-exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 12:19:32 by omartela          #+#    #+#             */
/*   Updated: 2024/10/02 22:02:50 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_numeric(char *str)
{
	int	i;

	i = 0;
	if (str[0] == '-' || str[0] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	exit_shell(t_shell *sh, char **args)
{
	int	exit_code;

	ft_printf("exit\n");
	if (args[1] && args[2])
	{
		ft_putstr_fd("exit: too many arguments\n", 2);
		sh->exit_status = 1;
		return (1);
	}
	if (args[1] && !is_numeric(args[1]))
	{
		ft_putstr_fd("exit: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		free_shell(sh);
		exit(255);
	}
	if (args[1])
		exit_code = ft_atoi(args[1]);
	else
		exit_code = sh->exit_status;
	free_shell(sh);
	exit(exit_code);
}
