/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_split.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 13:00:40 by irychkov          #+#    #+#             */
/*   Updated: 2024/09/15 12:38:53 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	test_split_args_without_quotes(char *input, char c)
{
	char	**result;
	int		i;

	i = 0;
	result = split_args_without_quotes(input, c);
	while (result[i])
	{
		printf("arg[%d]: %s\n", i, result[i]);
		free(result[i]);
		i++;
	}
	free(result);
}

void	test_split_args_with_quotes(char *input, char c)
{
	char	**result;
	int		i;

	i = 0;
	result = split_args_with_quotes(input, c);
	while (result[i])
	{
		printf("arg[%d]: %s\n", i, result[i]);
		free(result[i]);
		i++;
	}
	free(result);
}
