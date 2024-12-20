/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input_utilities2.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 14:58:40 by irychkov          #+#    #+#             */
/*   Updated: 2024/11/04 12:46:35 by irychkov         ###   ########.fr       */
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
		sh->promtflag = 0;
		if (sh->promt && sh->promt[0] != '\0')
			add_history(sh->promt);
		free(*input);
		return (1);
	}
	*input = trimmed_input;
	return (0);
}

static int	join_with_open_quote(t_shell *sh, char **input, char *next_input)
{
	char	*temp;
	char	*joined_input;

	joined_input = NULL;
	temp = NULL;
	temp = ft_strjoin(*input, "\n");
	free(*input);
	if (!temp)
	{
		free(next_input);
		error_sys("ft_strjoin failed\n");
		sh->exit_status = 1;
		return (1);
	}
	joined_input = ft_strjoin(temp, next_input);
	free(temp);
	free(next_input);
	if (!joined_input)
	{
		error_sys("ft_strjoin failed\n");
		sh->exit_status = 1;
		return (1);
	}
	*input = joined_input;
	return (0);
}

static int	join_without_open_quote(t_shell *sh, char **input, char *next_input)
{
	char	*joined_input;

	joined_input = NULL;
	joined_input = ft_strjoin(*input, next_input);
	free(*input);
	free(next_input);
	if (!joined_input)
	{
		error_sys("ft_strjoin failed\n");
		sh->exit_status = 1;
		return (1);
	}
	*input = joined_input;
	return (0);
}

int	join_input_with_next(t_shell *sh, char **input, char *next_input)
{
	if (is_open_quote(*input))
		return (join_with_open_quote(sh, input, next_input));
	else
		return (join_without_open_quote(sh, input, next_input));
}
