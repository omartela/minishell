/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parse_redirect.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 22:30:45 by irychkov          #+#    #+#             */
/*   Updated: 2024/09/04 22:32:55 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_command(t_cmd *cmd)
{
	printf("Command: ");
	for (int i = 0; cmd->args[i] != NULL; i++) {
		printf("%s ", cmd->args[i]);
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
