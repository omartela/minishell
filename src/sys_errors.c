/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_errors.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 11:02:11 by irychkov          #+#    #+#             */
/*   Updated: 2024/10/22 10:17:05 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	error_sys(char *msg)
{
	write(2, msg, ft_strlen(msg));
}

void	error_dup(t_shell *sh, t_cmd *cmd)
{
	error_sys("dup2 failed\n");
	exit_and_free(sh, cmd, 1);
}
