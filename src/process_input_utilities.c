/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input_utilities.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 16:19:37 by irychkov          #+#    #+#             */
/*   Updated: 2024/11/04 11:38:33 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_open_quote(char *str)
{
	int	i;
	int	in_single_quotes;
	int	in_double_quotes;

	i = 0;
	in_single_quotes = 0;
	in_double_quotes = 0;
	while (str[i])
	{
		if (str[i] == '\"' && !in_single_quotes)
			in_double_quotes = !in_double_quotes;
		if (str[i] == '\'' && !in_double_quotes)
			in_single_quotes = !in_single_quotes;
		i++;
	}
	if (in_single_quotes || in_double_quotes)
		return (1);
	return (0);
}

int	trim_and_check_syntax(t_shell *sh, char **input)
{
	char	*trimmed_input;

	trimmed_input = trim_spaces(*input);
	if (check_syntax(trimmed_input))
	{
		sh->exit_status = 2;
		sh->promtflag = 0;
		if (sh->promt && sh->promt[0] != '\0')
			add_history(sh->promt);
		free(*input);
		return (1);
	}
	*input = trimmed_input;
	return (0);
}

int	expand_and_add_spaces(t_shell *sh, char **input)
{
	char	*expanded_input;
	char	*spaced_input;

	expanded_input = expand_input(*input, sh);
	free(*input);
	if (!expanded_input)
	{
		error_sys("expand_input failed\n");
		sh->exit_status = 1;
		return (1);
	}
	spaced_input = add_spaces(expanded_input);
	free(expanded_input);
	if (!spaced_input)
	{
		error_sys("add_spaces failed\n");
		sh->exit_status = 1;
		return (1);
	}
	*input = spaced_input;
	return (0);
}

static int	process_next_input(t_shell *sh, char **input,
			char *next_input, int saved_stdin)
{
	if (add_prompt(sh, " ") || add_prompt(sh, next_input))
	{
		free(*input);
		free(next_input);
		return (1);
	}
	if (trim_and_check_syntax(sh, &next_input)
		|| expand_and_add_spaces(sh, &next_input))
	{
		free(*input);
		return (1);
	}
	if (handle_heredoc_if_needed(sh, next_input, saved_stdin))
	{
		free(*input);
		return (1);
	}
	if (join_input_with_next(sh, input, next_input))
		return (1);
	return (0);
}

int	handle_continued_input(t_shell *sh, char **input, int len, int saved_stdin)
{
	char	*next_input;

	next_input = NULL;
	g_sig = 0;
	signal(SIGINT, signal_handler_hd);
	while ((len > 0 && (*input)[len - 1] == '|')
		|| (len > 2 && (*input)[len - 1] == '&'
		&& (*input)[len - 2] == '&') || (len > 0 && is_open_quote(*input)))
	{
		//Snippet for tester
		if (isatty(fileno(stdin)))
			next_input = readline("> ");
		else
		{
			char *line = get_next_line(fileno(stdin));
			next_input = ft_strtrim(line, "\n");
			free(line);
		}
		//next_input = readline("> ");
		if (g_sig == SIGINT)
		{
			free(*input);
			return (-2);
		}
		if (!next_input && is_open_quote(*input))
		{
			error_sys("syntax error: unexpected end of file\n");
			sh->promtflag = 1;
			free(*input);
			return (0);
		}
		else if (!next_input)
		{
			error_sys("syntax error: unexpected end of file\n");
			sh->promtflag = 1;
			free(*input);
			return (-1);
		}
		char *temp = ft_strdup(next_input);
		free(next_input);
		next_input = temp;
		if (process_next_input(sh, input, next_input, saved_stdin))
			return (0);
		len = ft_strlen(*input);
	}
	return (1);
}
