/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 14:07:16 by omartela          #+#    #+#             */
/*   Updated: 2024/09/04 18:23:02 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	userprompt(char **envp)
{
	char	*input;
	char	**commands;

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
		test_split(input);
		commands = ft_split(input, '|');
		if (commands)
		{
			if (execute_pipes(commands, envp) == 1)
				perror("comment"); //TBH we don't care about the return value
			free_array(commands);
		}
		else
			perror("ft_split failed");
		free(input);
	}
}

int	main(int ac, char **av, char **envp)
{
	(void)ac;
	(void)av;
	userprompt(envp);
}
