/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_split.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 13:00:40 by irychkov          #+#    #+#             */
/*   Updated: 2024/09/04 13:51:07 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
first commit
minishell> libft "ls -la" 'grep "{print $1}"'
You have entered: libft "ls -la" 'grep "{print $1}"'
arg[0]: libft
arg[1]: "ls -la"
arg[2]: 'grep "{print $1}"'

second commit
minishell> libft "ls -la" 'grep "{print $1}"'
You have entered: libft "ls -la" 'grep "{print $1}"'
arg[0]: libft
arg[1]: ls -la
arg[2]: grep {print $1}
 */

void	test_split(char *input)
{
	char	**result;
	int		i;

	i = 0;
	result = ft_split_args(input, ' ');
	while (result[i])
	{
		printf("arg[%d]: %s\n", i, result[i]);
		free(result[i]);
		i++;
	}
	free(result);
}
