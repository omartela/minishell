/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin-unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omartela <omartela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 16:05:55 by omartela          #+#    #+#             */
/*   Updated: 2024/09/23 16:06:53 by omartela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../include/minishell.h"

int unset(t_shell *sh, char **args)
{
    int argc;
    int i;
    /// add error checking for args
    argc = 0;
    i = 1;
    while (args[argc])
        ++argc;
    if (argc >= 2)
    {
        while (i < argc)
        {
            remove_table(&sh->envp, args[i]);
            remove_table(&sh->local_shellvars, args[i]);
            ++i;
        }
    }
    return (0);
}
