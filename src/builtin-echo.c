/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin-echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 14:29:58 by omartela          #+#    #+#             */
/*   Updated: 2024/10/10 19:44:27 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	check_dash_n(char *argv)
{
	if (*argv != '-' || ft_strncmp(argv, "-\0", 2) == 0)
		return (1);
	else
		++argv;
	while (*argv)
	{
		if (*argv != 'n')
			return (1);
		++argv;
	}
	return (0);
}

static void	add_new_line(int no_nl)
{
	if (!no_nl)
		write(1, "\n", 1);
}

static int	echo_print_str(char **argv, int i, int *check_options)
{
	while (argv[i])
	{
		if (argv[i] && (ft_strncmp(argv[i], "-\0", 2) == 0))
		{
			ft_putstr_fd(argv[i], 1);
			if (argv[i + 1])
				write(1, " ", 1);
			++i;
			continue ;
		}
		if (!check_dash_n(argv[i]) && *check_options)
			++i;
		else
		{
			*check_options = 0;
			ft_putstr_fd(argv[i], 1);
			if (argv[i + 1])
				write(1, " ", 1);
			++i;
		}
	}
	return (0);
}

int	echo(char *argv[])
{
	int	i;
	int	no_nl;
	int check_options;

	check_options = 1;
	i = 1;
	no_nl = 0;
	if (!argv[1])
	{
		write(1, "\n", 1);
		return (0);
	}
	if (!check_dash_n(argv[1]))
	{
		i = 2;
		no_nl = 1;
	}
	else
		check_options = 0;
	if (echo_print_str(argv, i, &check_options))
		return (1);
	add_new_line(no_nl);
	return (0);
}
