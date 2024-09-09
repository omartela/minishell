/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_echo.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omartela <omartela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 15:19:28 by omartela          #+#    #+#             */
/*   Updated: 2024/09/05 15:34:48 by omartela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	test_echo_command(char *argv[], t_shell *shell)
{
	pid_t	pid;

	printf("Test echo command \n");
	echo_command(argv);
	pid = fork();
	if (pid == 0)
		execve("/bin/echo", argv, shell->envp);
}
