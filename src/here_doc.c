/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 12:56:24 by irychkov          #+#    #+#             */
/*   Updated: 2024/10/31 12:11:58 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_heredoc(char *input)
{
	int	i;
	int	in_single_quotes;
	int	in_double_quotes;

	i = 0;
	in_single_quotes = 0;
	in_double_quotes = 0;
	while (input[i])
	{
		if (input[i] == '\"' && !in_single_quotes)
			in_double_quotes = !in_double_quotes;
		if (input[i] == '\'' && !in_double_quotes)
			in_single_quotes = !in_single_quotes;
		if (input[i] == '<' && !in_double_quotes && !in_single_quotes)
		{
			if (input[i + 1] && input[i + 1] == '<')
				return (1);
		}
		i++;
	}
	return (0);
}

int	handle_here_doc(t_shell *sh, char *input)
{
	int		catch_error;
	char	**args;
	char	**args_with_quotes;
	int		expand_flag;

	expand_flag = 0;
	args = split_args_remove_quotes(input, ' ');
	if (!args)
		return (1);
	args_with_quotes = split_args_leave_quotes(input, ' ');
	if (!args_with_quotes)
	{
		free(args);
		return (1);
	}
	catch_error = loop_args(sh, args, args_with_quotes, &expand_flag);
	if (catch_error == 1)
		return (1);
	else if (catch_error == -1)
		return (-1);
	free_array(&args);
	free_array(&args_with_quotes);
	return (0);
}
