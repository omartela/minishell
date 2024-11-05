/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 11:18:27 by omartela          #+#    #+#             */
/*   Updated: 2024/11/05 17:46:20 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	sig_handler_sigint_2(int signum)
{
	if (signum == SIGINT)
	{
		printf("\n");
	}
	if (signum == SIGQUIT)
	{
		error_sys("Quit\n");
	}
}

static void	sig_handler_sigint(int signum)
{
	if (signum == SIGINT)
	{
		g_sig = signum;
		rl_replace_line("", 0);
		printf("\n");
		rl_on_new_line();
		rl_redisplay();
	}
}

void	reset_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	change_signal_handler(void)
{
	signal(SIGINT, sig_handler_sigint_2);
	signal(SIGQUIT, sig_handler_sigint_2);
}

void	init_signal_first(void)
{
	signal(SIGINT, sig_handler_sigint);
	signal(SIGQUIT, SIG_IGN);
}
