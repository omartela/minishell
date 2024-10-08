/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin-echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 14:29:58 by omartela          #+#    #+#             */
/*   Updated: 2024/10/02 16:19:07 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	check_dash_n(char *argv)
{
	if (*argv != '-')
	{
		return (1);
	}
	else
		++argv;
	while (*argv)
	{
		if (*argv != 'n')
		{
			return (1);
		}
		++argv;
	}
	return (0);
}

int	echo(char *argv[])
{
	int	i;
	int	no_nl;

	i = 1;
	no_nl = 0;
	if (!argv[1])
	{
		write(1, "\n", 1);
		return (0);
	}
	if (check_dash_n(argv[1]))
	{
		i = 2;
		no_nl = 1;
	}
	while (argv[i])
	{
		if (!check_dash_n(argv[i]))
			++i;
		else
		{
			ft_putstr_fd(argv[i], 1);
			if (argv[i + 1])
				write(1, " ", 1);
			++i;
		}
	}
	if (!no_nl)
		write(1, "\n", 1);
	return (0);
}
