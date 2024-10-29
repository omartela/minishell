/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 11:18:27 by omartela          #+#    #+#             */
/*   Updated: 2024/10/24 18:58:59 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	sig_handler_sigint_2(int signum)
{
	if (signum == SIGINT || signum == SIGQUIT)
	{
		printf("\n");
	}
}

static void	sig_handler_sigint(int signum)
{
	if (signum == SIGINT)
	{
		rl_replace_line("", 0);
		printf("\n");
		rl_on_new_line();
		rl_redisplay();
	}
}

int	reset_signals()
{
	signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, SIG_DFL);
	return (0);
}

int	change_signal_handler(void)
{
	signal(SIGINT, sig_handler_sigint_2);
	signal(SIGQUIT, sig_handler_sigint_2);
	return (0);
}

int	init_signal()
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, sig_handler_sigint);
	return (0);
}
