/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 15:43:09 by omartela          #+#    #+#             */
/*   Updated: 2024/10/07 17:15:56 by omartela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	initialize_shell(t_shell *sh, char **envp)
{
	sh->exit_status = 0;
	sh->num_cmds = 0;
	sh->commands = NULL;
	sh->heredoc_fds = NULL;
	sh->num_heredocs = 0;
	sh->heredoc_index = 0;
	copy_env(envp, sh);
	sh->pid = ft_get_pid();
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
	char 	*split_input;
	char	*next_input;

	next_input = NULL;
	split_input = NULL;
	input = trim_spaces(input);
	if (check_syntax(input))
	{
		sh->exit_status = 2;
		return ;
	}
	split_input = ft_add_spaces(input);
	free(input);
	input = split_input;
	if (is_heredoc(input))
		handle_here_doc(sh, input);
	len = ft_strlen(input);
	while (input[len - 1] == '|' || (len > 2 && input[len - 1] == '&' && input[len - 2] == '&'))
	{
		next_input = readline("> ");
		if (!next_input)
		{
			free(input);
			//printf("Exit \n");
			return ;//nor sure if this is the right way to exit
		}
		//Snippet for tester
		/* if (isatty(fileno(stdin)))
			next_input = readline("> ");
		else
		{
			char *line;
			line = get_next_line(fileno(stdin));
			next_input = ft_strtrim(line, "\n");
			free(line);
		} */
		next_input = trim_spaces(next_input);
		if (check_syntax(next_input))
		{
			free(input);
			free(next_input);
			sh->exit_status = 2;
			return ;
		}
		split_input = ft_add_spaces(next_input);
		free(next_input);
		next_input = split_input;
		if (is_heredoc(next_input))
			handle_here_doc(sh, next_input);
		input = ft_strjoin(input, next_input);
		if (!input)
		{
			error_sys("ft_strjoin failed\n");
			sh->exit_status = 1;
			free(next_input);
			return ;
		}
		len = ft_strlen(input);
		/* free(temp);
		free(next_input); */ //SegFAULT if I free here. I should think
	}
	if (*input)
		add_history(input);
	/* if (temp)// think about this, we free it in userprompt
		free(temp); */
	if (next_input)
		free(next_input);
/* 	test_split_args_leave_quotes(input , '|'); // Only for testing */
	sh->commands = split_args_leave_quotes(input, '|');
	free(input);
	if (sh->commands)
	{
		init_num_cmds(sh);
/* 		printf("Number of commands: %d\n", sh->num_cmds); // Only for testingd */
		execute_pipes(sh);
		//free_array(sh->commands);
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
		//Snippet for tester
/* 		if (isatty(fileno(stdin)))
			input = readline("minishell> ");
		else
		{
			char *line;
			line = get_next_line(fileno(stdin));
			input = ft_strtrim(line, "\n");
			free(line);
		} */
		input = readline("minishell> ");
		if (input == NULL)
		{
			//printf("Exit \n");
			break ;
		}
		process_input(&sh, input);
	}
	free_shell(&sh);
}

int	main(int ac, char **av, char **envp)
{
	(void)ac;
	(void)av;
	userprompt(envp);
}
