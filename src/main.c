/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 15:43:09 by omartela          #+#    #+#             */
/*   Updated: 2024/09/06 18:05:30 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	initialize_shell(t_shell *sh, char **envp)
{
	sh->num_cmds = 0;
	sh->commands = NULL;
	sh->envp = envp;
	sh->homepath = ft_strdup(getenv("HOME")); // Protect this
}

static void	process_input(t_shell *sh, char *input)
{
	if (*input)
		add_history(input);
	printf("You have entered: %s\n", input); // Only for testing
	test_split(input); // Only for testing
	sh->commands = ft_split(input, '|');
	if (sh->commands)
	{
		init_num_cmds(sh);
		if (execute_pipes(sh) == 1)
			perror("comment"); //TBH we don't care about the return value
		free_array(sh->commands);
	}
	else
		perror("ft_split failed");
}

static void	userprompt(char **envp)
{
	t_shell	sh;
	char	*input;

	initialize_shell(&sh, envp);
	while (1)
	{
		input = readline("minishell> ");
		if (input == NULL)
		{
			printf("Exit \n");
			break ;
		}
		process_input(&sh, input);
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
