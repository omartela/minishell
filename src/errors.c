/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 11:35:41 by irychkov          #+#    #+#             */
/*   Updated: 2024/10/23 10:48:41 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	show_error(char *name, char *msg)
{
	if (name && name[0] != '\0')
		write(2, name, ft_strlen(name));
	write(2, ": ", 2);
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
}

void	show_error_free_cmd_exit(int code, char *name, char *msg, t_cmd *cmd)
{
	if (name && name[0] != '\0')
		write(2, name, ft_strlen(name));
	write(2, ": ", 2);
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
	if (cmd)
		free_cmd(&cmd);
	exit(code);
}

int	show_error_return(int code, char *name, char *msg)
{
	if (name && name[0] != '\0')
		write(2, name, ft_strlen(name));
	write(2, ": ", 2);
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
	return (code);
}

void	exit_and_free(t_shell *sh, t_cmd *cmd, int status)
{
	free_cmd(&cmd);
	free_shell(sh);
	exit(status);
}
