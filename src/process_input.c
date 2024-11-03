/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 16:16:43 by irychkov          #+#    #+#             */
/*   Updated: 2024/11/03 15:47:25 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	handle_heredoc_if_needed(t_shell *sh, char *input, int saved_stdin)
{
	int	catch_error;

	catch_error = 0;
	if (!is_heredoc(input))
		return (0);
	catch_error = handle_here_doc(sh, input);
	if (catch_error == 1)
	{
		error_sys("handle_here_doc failed\n");
		free(input);
		return (1);
	}
	if (catch_error == -1)
	{
		if (dup2(saved_stdin, STDIN_FILENO) == -1)
			error_sys("dup2 failed to restore STDIN\n");
		close(saved_stdin);
		printf("\n");
		free(input);
		return (1);
	}
	return (0);
}

static void	finalize_input(t_shell *sh, char **input)
{
	if (sh->promt && sh->promt[0] != '\0')
		add_history(sh->promt);
	sh->commands = split_args_leave_quotes(*input, '|');
	free(*input);
	if (sh->commands)
	{
		init_num_cmds(sh);
		if (sh->num_cmds > 0)
			execute_pipes(sh);
		else
			sh->exit_status = 0;
	}
	else
	{
		error_sys("split_args_leave_quotes failed\n");
		sh->exit_status = 1;
	}
}

int	process_input(t_shell *sh, char *input)
{
	int	len;
	int	saved_stdin;
	int	check_eof;

	if (trim_and_check_syntax(sh, &input))
		return (0);
	if (expand_and_add_spaces(sh, &input))
		return (0);
	saved_stdin = dup(STDIN_FILENO);
	if (saved_stdin == -1)
	{
		error_sys("dup failed\n");
		return (0);
	}
	if (handle_heredoc_if_needed(sh, input, saved_stdin))
		return (0);
	len = ft_strlen(input);
	check_eof = handle_continued_input(sh, &input, len, saved_stdin);
	if (check_eof == -1 || check_eof == 0)
	{
		close(saved_stdin);
		return (check_eof);
	}
	g_sig = 0;
	close(saved_stdin);
	finalize_input(sh, &input);
	return (0);
}
