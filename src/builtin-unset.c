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
    /// add error checking for args
    remove_table(&sh->envp, args[1]);
    remove_table(&sh->local_shellvars, args[1]);
    return (0);
}