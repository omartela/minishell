/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 15:43:09 by omartela          #+#    #+#             */
/*   Updated: 2024/10/29 10:39:47 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	add_prompt(t_shell *sh, char *input)
{
	char	*temp;

	if (sh->promt)
	{
		temp = ft_strjoin(sh->promt, input);
		if (!temp)
		{
			error_sys("add_prompt failed\n");
			return (1);
		}
		free(sh->promt);
		sh->promt = temp;
	}
	else
	{
		sh->promt = ft_strdup(input);
		if (!sh->promt)
		{
			error_sys("add_prompt failed\n");
			return (1);
		}
	}
	return (0);
}

static void	loop_userpromt(t_shell *sh)
{
	char	*input;

	while (1)
	{
		if (init_signal(sh))
		{
			error_sys("Init signals failed\n");
			continue ;
		}
		input = readline("minishell> ");
		if (input == NULL)
		{
			printf("exit\n");
			break ;
		}
		if (add_prompt(sh, input))
		{
			free(input);
			continue ;
		}
		process_input(sh, input);
		free_partial(sh);
	}
}

static int	userprompt(int status, char ***envp)
{
	t_shell	sh;

	initialize_shell(&sh, envp);
	loop_userpromt(&sh);
	status = sh.exit_status;
	free_shell(&sh);
	rl_clear_history();
	return (status);
}

int	main(int ac, char **av, char **envp)
{
	int	status;

	status = 0;
	if (ac != 1)
	{
		printf("The minishell doesn't handle arguments\n");
		return (0);
	}
	(void)av;
	return (userprompt(status, &envp));
}
