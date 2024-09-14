/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 15:43:09 by omartela          #+#    #+#             */
/*   Updated: 2024/09/14 12:24:31 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	initialize_shell(t_shell *sh, char **envp)
{
	sh->num_cmds = 0;
	sh->commands = NULL;
	sh->envp = envp;
	sh->homepath = ft_strdup(getenv("HOME"));
	if (!sh->homepath)
	{
		error_sys("ft_strdup failed for getenv\n");
		exit (1);
	}
}

static void	process_input(t_shell *sh, char *input)
{
	int		len;
	char	*next_input;

/* 	printf("You have entered: %s\n", input); // Only for testing
	test_split(input); // Only for testing */
	input = trim_spaces(input);
	len = ft_strlen(input);
	if (check_syntax(input))
		return ;
	while (input[len - 1] == '|' || (len > 2 && input[len - 1] == '&' && input[len - 2] == '&'))
	{
		next_input = readline("> ");
		if (!next_input)
		{
			free(input);
			printf("Exit \n");
			return ;//nor sure if this is the right way to exit
		}
		char *temp = input;
		input = ft_strjoin(input, next_input);
		free(temp);
		free(next_input);
		trim_spaces(input);
		len = ft_strlen(input);
		if (check_syntax(input))
			return ;
	}
	if (*input)
		add_history(input);
	test_split_args(input , '|'); // Only for testing
	sh->commands = ft_split_args(input, '|');
	if (sh->commands)
	{
		init_num_cmds(sh);
/* 		printf("Number of commands: %d\n", sh->num_cmds); // Only for testingd */
		if (execute_pipes(sh) == 1)
			perror("comment"); //TBH we don't care about the return value
		free_array(sh->commands);
	}
	else
	{
		error_sys("ft_split failed\n"); //do we need to return something?
	}
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
