/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parse_redirect.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 22:30:45 by irychkov          #+#    #+#             */
/*   Updated: 2024/09/15 12:58:40 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_command(t_cmd *cmd)
{
	if (cmd == NULL)
	{
		printf("Error: cmd is NULL\n");
		return ;
	}
	if (cmd->args == NULL)
	{
		printf("Error: cmd->args is NULL\n");
		return ;
	}
	printf("Command: \n");
	for (int i = 0; cmd->args[i] != NULL; i++) {
		printf("cmd[%d] %s\n", i, cmd->args[i]);
	}
	printf("\n");
	if (cmd->infile)
		printf("Input file: %s\n", cmd->infile);
	if (cmd->outfile) 
	{
		printf("Output file: %s\n", cmd->outfile);
		if (cmd->append)
			printf("Append mode (>>)\n");
		else
			printf("Overwrite mode (>)\n");
	}
}
