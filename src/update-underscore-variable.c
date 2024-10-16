/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update-underscore-variable.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omartela <omartela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 09:27:13 by omartela          #+#    #+#             */
/*   Updated: 2024/10/16 09:27:16 by omartela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void    update_underscore_variable_path(t_shell *sh, t_cmd *cmd)
{
    set_variables(sh, "_", cmd->path);
}

void    update_underscore_variable_argument(t_shell *sh, t_cmd *cmd)
{
    int i;

    while (cmd->args[i])
        ++i;
    set_variables(sh, "_", cmd->args[i]);
}