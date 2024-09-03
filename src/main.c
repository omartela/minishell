/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omartela <omartela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 14:07:16 by omartela          #+#    #+#             */
/*   Updated: 2024/09/03 14:34:23 by omartela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../include/minishell.h"

void	userprompt(void)
{
	char	*input;

	while (1)
	{
		input = readline("minishell> ");
		if (input == NULL)
		{
			printf("Exit \n");
			break ;
		}
		if (*input)
		{
			add_history(input);
		}
		printf("You have entered: %s\n", input);
		free(input);
	}
}

int	main(void)
{
	userprompt();
}
