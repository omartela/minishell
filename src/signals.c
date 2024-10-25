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
	if (signum == SIGINT)
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

int	reset_signals(t_shell *sh)
{
	if (sigaction(SIGINT, &sh->org_sig_int, NULL) == -1)
	{
		error_sys("Error resetting SIGINT handler\n");
		return (1);
	}
	if (sigaction(SIGQUIT, &sh->org_sig_quit, NULL) == -1)
	{
		error_sys("Error resetting SIGQUIT handler\n");
		return (1);
	}
	return (0);
}

int	change_signal_handler(void)
{
	struct sigaction	sa_int;

	ft_memset(&sa_int, 0, sizeof(sa_int));
	if (sigemptyset(&sa_int.sa_mask) == -1)
		return (1);
	sa_int.sa_flags = SA_RESTART;
	sa_int.sa_handler = sig_handler_sigint_2;
	if (sigaction(SIGINT, &sa_int, NULL) == -1)
		return (1);
	return (0);
}

int	init_signal(t_shell *sh)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	ft_memset(&sa_int, 0, sizeof(sa_int));
	if (sigemptyset(&sa_int.sa_mask) == -1)
		return (1);
	sa_int.sa_flags = SA_RESTART;
	sa_int.sa_handler = sig_handler_sigint;
	if (sigaction(SIGINT, &sa_int, &sh->org_sig_int) == -1)
		return (1);
	ft_memset(&sa_quit, 0, sizeof(sa_quit));
	if (sigemptyset(&sa_quit.sa_mask) == -1)
		return (1);
	sa_quit.sa_flags = 0;
	sa_quit.sa_handler = SIG_IGN;
	if (sigaction(SIGQUIT, &sa_quit, &sh->org_sig_quit) == -1)
		return (1);
	return (0);
}
