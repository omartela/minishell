/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 15:43:09 by omartela          #+#    #+#             */
/*   Updated: 2024/11/04 13:16:02 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	g_sig = 0;

void	init_signal(t_shell *sh)
{
	if (g_sig == 2 || sh->promtflag)
		init_signal_updated();
	else
		init_signal_first();
}

static void	loop_userpromt(t_shell *sh)
{
	char	*input;

	while (1)
	{
		init_signal(sh);
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
		if (process_input(sh, input))
		{
			printf("exit\n");
			break ;
		}
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
