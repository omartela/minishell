/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 16:16:43 by irychkov          #+#    #+#             */
/*   Updated: 2024/10/24 16:31:08 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	trim_and_check_syntax(t_shell *sh, char **input)
{
	char	*trimmed_input;

	trimmed_input = trim_spaces(*input);
	if (check_syntax(trimmed_input))
	{
		sh->exit_status = 2;
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

int	handle_heredoc_if_needed(t_shell *sh, char *input)
{
	if (is_heredoc(input))
	{
		if (handle_here_doc(sh, input))
		{
			error_sys("handle_here_doc failed\n");
			free(input);
			return (1);
		}
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

void	process_input(t_shell *sh, char *input)
{
	int	len;

	if (trim_and_check_syntax(sh, &input))
		return ;
	if (expand_and_add_spaces(sh, &input))
		return ;
	if (handle_heredoc_if_needed(sh, input))
		return ;
	len = ft_strlen(input);
	if (handle_continued_input(sh, &input, len))
		return ;
	finalize_input(sh, &input);
}
