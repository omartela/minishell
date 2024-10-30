/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 12:56:24 by irychkov          #+#    #+#             */
/*   Updated: 2024/10/30 22:09:12 by irychkov         ###   ########.fr       */
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

/* static int	loop_args(t_shell *sh, t_heredoc_args *hd_args, int *expand)
{
	int	i;
	int	catch_error;

	catch_error = 0;
	i = 0;
	while (hd_args->args[i])
	{
		*expand = 0;
		if (ft_strncmp(hd_args->args[i], "<<\0", 3) == 0
			&& hd_args->args[i + 1])
		{
			if ((ft_strncmp(hd_args->args[i + 1],
						hd_args->args_with_quotes[i + 1],
						ft_strlen(hd_args->args[i + 1])) == 0)
				|| hd_args->args_with_quotes[i + 1][0] == '\"')
				*expand = 1;
			if (allocate_heredocs_fds(sh->hd, hd_args->args))
			{
				free_array(&hd_args->args_with_quotes);
				return (1);
			}
			catch_error = save_heredoc_fds(sh, hd_args, expand, i);
			if (catch_error == 1)
			{
				free_array(&hd_args->args_with_quotes);
				return (1);
			}
			else if (catch_error == -1)
			{
				free_array(&hd_args->args_with_quotes);
				return (-1);
			}
			sh->hd->num_heredocs++;
			i += 2;
		}
		else
			i++;
	}
	return (0);
} */

int	handle_here_doc(t_shell *sh, char *input)
{
	t_heredoc_args	hd_args;
	int				expand_flag;
	int				catch_error;

	ft_memset(&hd_args, 0, sizeof(t_heredoc_args));
	hd_args.input = input;
	expand_flag = 0;
	hd_args.args = split_args_remove_quotes(hd_args.input, ' ');
	if (!hd_args.args)
		return (1);
	hd_args.args_with_quotes = split_args_leave_quotes(hd_args.input, ' ');
	if (!hd_args.args_with_quotes)
	{
		free(hd_args.args);
		return (1);
	}
	catch_error = loop_args(sh, &hd_args, &expand_flag);
	if (catch_error == 1)
		return (1);
	else if (catch_error == -1)
		return (-1);
	free_array(&hd_args.args);
	free_array(&hd_args.args_with_quotes);
	return (0);
}
