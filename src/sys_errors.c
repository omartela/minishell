/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_errors.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 11:02:11 by irychkov          #+#    #+#             */
/*   Updated: 2024/10/28 11:48:11 by irychkov         ###   ########.fr       */
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

void	error_execve_and_permission(t_shell *sh, t_cmd *cmd, int is_abs_rel)
{
	check_permissions(sh, cmd, is_abs_rel);
	error_sys("execve failed\n");
	exit_and_free(sh, cmd, 1);
}

void	show_syntax_error(char *element, char *input)
{
	add_history(input);
	error_sys("minishell: syntax error near unexpected token `");
	write(2, element, ft_strlen(element));
	write(2, "'\n", 2);
}
