/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_utilities.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 12:26:44 by irychkov          #+#    #+#             */
/*   Updated: 2024/11/04 12:35:22 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	signal_handler_hd(int signal)
{
	close(STDIN_FILENO);
	g_sig = signal;
}

static void	sig_handler_sigint_g(int signum)
{
	if (signum == SIGINT)
	{
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	init_signal_updated(void)
{
	signal(SIGINT, sig_handler_sigint_g);
	signal(SIGQUIT, SIG_IGN);
}
