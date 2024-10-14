/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 15:43:09 by omartela          #+#    #+#             */
/*   Updated: 2024/10/14 17:43:05 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	initialize_shell(t_shell *sh, char **envp)
{
	t_heredoc	*hd;

	sh->exit_status = 0;
	sh->num_cmds = 0;
	sh->commands = NULL;
	sh->pipes = NULL;
	copy_env(envp, sh);
	sh->homepath = expand(envp, "HOME");
	if (!sh->homepath)
	{
		error_sys("ft_strdup failed for getenv\n");
		exit (1);
	}
	hd = malloc(sizeof(t_heredoc));
	if (!hd)
	{
		error_sys("malloc failed for t_heredoc\n");
		exit(1);
	}
	sh->hd = hd;
	hd->heredoc_fds = NULL;
	hd->num_heredocs = 0;
	hd->heredoc_index = 0;
	ft_memset(&sh->org_sig_int, 0, sizeof(sh->org_sig_int));
	ft_memset(&sh->org_sig_quit, 0, sizeof(sh->org_sig_quit));
	if (init_signal(sh))
		exit (1);
}

static void	process_input(t_shell *sh, char *input)
{
	int		len;
	char 	*split_input;
	char	*next_input;

	next_input = NULL;
	split_input = NULL;

	split_input = trim_spaces(input);
	if (check_syntax(split_input))
	{
		sh->exit_status = 2;
		return ;
	}
	input = split_and_parse(split_input, sh);
	if (!input)
	{
		error_sys("split_and_parse failed\n");
		sh->exit_status = 1;
		return ;
	}
	split_input = ft_add_spaces(input);
	free(input);
	if (!split_input)
	{
		error_sys("ft_add_spaces failed\n");
		sh->exit_status = 1;
		return ;
	}
	input = split_input;
	if (is_heredoc(input))
	{
		if (handle_here_doc(sh, input))
		{
			error_sys("handle_here_doc failed\n");
			free(input);
			return ;
		}
	}
	len = ft_strlen(input);
	while ((len > 0 && input[len - 1] == '|') || (len > 2 && input[len - 1] == '&' && input[len - 2] == '&'))
	{
		next_input = readline("> ");
		if (!next_input)
		{
			free(input);
			//printf("Exit \n");
			return ;
		}
/* 		//Snippet for tester
		if (isatty(fileno(stdin)))
			next_input = readline("> ");
		else
		{
			char *line;
			line = get_next_line(fileno(stdin));
			next_input = ft_strtrim(line, "\n");
			free(line);
		} */
		if (!next_input)
		{
			free(input);
			//printf("Exit \n");
			return ;
		}
		split_input = split_and_parse(next_input, sh);
		free(next_input);
		if (!split_input)
		{
			error_sys("split_and_parse failed\n");
			free(input);
			sh->exit_status = 1;
			return ;
		}
		next_input = trim_spaces(split_input);
		if (check_syntax(next_input))
		{
			free(input);
			free(next_input);
			sh->exit_status = 2;
			return ;
		}
		split_input = ft_add_spaces(next_input);
		free(next_input);
		if (!split_input)
		{
			error_sys("ft_add_spaces failed\n");
			free(input);
			sh->exit_status = 1;
			return ;
		}
		next_input = split_input;
		if (is_heredoc(next_input))
		{	
			if (handle_here_doc(sh, next_input))
			{
				error_sys("handle_here_doc failed\n");
				free(input);
				free(next_input);
				return ;
			}
		}
		split_input = ft_strjoin(input, next_input);
		free(input);
		free(next_input);
		if (!split_input)
		{
			error_sys("ft_strjoin failed\n");
			sh->exit_status = 1;
			return ;
		}
		input = split_input;
		len = ft_strlen(input);
	}
	if (*input)
		add_history(input);
	if (next_input)
		free(next_input);
	sh->commands = split_args_leave_quotes(input, '|');
	free(input);
	if (sh->commands)
	{
		init_num_cmds(sh);
		if (sh->num_cmds == 0)
		{
			sh->exit_status = 0;
			return ;
		}
		execute_pipes(sh);
		free_partial(sh);
	}
	else
	{
		error_sys("ft_split failed\n");
		sh->exit_status = 1;
		return ;
	}
}

static int	userprompt(int status, char **envp)
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
		free(input);
	}
	status = sh.exit_status;
	free_shell(&sh);
	rl_clear_history();
	return (status);
}

int	main(int ac, char **av, char **envp)
{
	int	status;

	status = 0;
	(void)ac;
	(void)av;
	return (userprompt(status, envp));
}
