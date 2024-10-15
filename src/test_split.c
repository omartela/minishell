/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_split.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 13:00:40 by irychkov          #+#    #+#             */
/*   Updated: 2024/10/01 14:05:27 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	test_split_args_remove_quotes(char *input, char c)
{
	char	**result;
	int		i;

	i = 0;
	result = split_args_remove_quotes(input, c);
	while (result[i])
	{
		printf("arg[%d]: {%s}\n", i, result[i]);
		free(result[i]);
		i++;
	}
	free(result);
}

void	test_split_args_leave_quotes(char *input, char c)
{
	char	**result;
	int		i;

	i = 0;
	result = split_args_leave_quotes(input, c);
	while (result[i])
	{
		printf("arg[%d]: {%s}\n", i, result[i]);
		free(result[i]);
		i++;
	}
	free(result);
}
