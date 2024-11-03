/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 15:43:09 by omartela          #+#    #+#             */
/*   Updated: 2024/11/03 15:52:24 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	g_sig = 0;

void	sig_handler_sigint_g(int signum)
{
	if (signum == SIGINT)
	{
		rl_on_new_line();
		rl_redisplay();
	}
}

static void	loop_userpromt(t_shell *sh)
{
	char	*input;

	while (1)
	{
		if (g_sig == 2)
			signal(SIGINT, sig_handler_sigint_g);
		else
			init_signal();
		//input = readline("minishell> ");
		if (isatty(fileno(stdin)))
			input = readline("minishell> ");
		else
		{
			char *line;
			line = get_next_line(fileno(stdin));
			input = ft_strtrim(line, "\n");
			free(line);
		}
		if (input == NULL)
		{
			/* printf("exit\n"); */
			break ;
		}
		char *temp = ft_strdup(input);
		free(input);
		input = temp;
		if (add_prompt(sh, input))
		{
			free(input);
			continue ;
		}
		if (process_input(sh, input))
		{
			/* printf("exit\n"); */
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
