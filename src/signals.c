/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omartela <omartela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 11:18:27 by omartela          #+#    #+#             */
/*   Updated: 2024/10/08 11:18:52 by omartela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void sig_handler_sigint(int signum)
{
    if (signum == SIGINT)
        printf("\nminishell> ");
}

int reset_signals(t_shell *sh) 
{
    if (sigaction(SIGINT, &sh->org_sig_int, NULL) == -1) 
    {
        perror("Error resetting SIGINT handler");
        return (1);
    }
    if (sigaction(SIGQUIT, &sh->org_sig_quit, NULL) == -1) 
    {
        perror("Error resetting SIGQUIT handler");
        return (1);
    }
    return (0);
}


int init_signal(t_shell *sh)
{
    struct sigaction    sa_int;
    struct sigaction    sa_quit;   
    

    ft_memset(&sa_int, 0, sizeof(sa_int));
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = SA_RESTART;
    sa_int.sa_handler = sig_handler_sigint;
    if (sigaction(SIGINT, &sa_int, &sh->org_sig_int) == -1)
    {
        printf("Error when initializing signal\n");
        return (1);
    }
    ft_memset(&sa_quit, 0, sizeof(sa_quit));
    sigemptyset(&sa_quit.sa_mask);
    sa_quit.sa_flags = 0;
    sa_quit.sa_handler = SIG_IGN;
    if (sigaction(SIGQUIT, &sa_quit, &sh->org_sig_quit) == -1)
    {
        printf("Error when initializing signal\n");
        return (1);
    }
    return (0);
}
