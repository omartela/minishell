/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 14:07:16 by omartela          #+#    #+#             */
/*   Updated: 2024/09/05 11:54:40 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	userprompt(char **envp)
{
	t_shell sh;
	char	*input;

	sh.commands = NULL;
	sh.envp = envp;
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
		sh.commands = ft_split(input, '|');
		if (sh.commands)
		{
			if (execute_pipes(&sh) == 1)
				perror("comment"); //TBH we don't care about the return value
			free_array(sh.commands);
		}
		else
			perror("ft_split failed");
		free(input);
	}
	free_shell(&sh);
}

int	main(int ac, char **av, char **envp)
{
	(void)ac;
	(void)av;
	userprompt(envp);
}