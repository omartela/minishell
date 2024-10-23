/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 12:56:24 by irychkov          #+#    #+#             */
/*   Updated: 2024/10/23 10:16:18 by irychkov         ###   ########.fr       */
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

static int	allocate_heredocs_fds(t_heredoc *hd, char **args)
{
	if (!hd->heredoc_fds)
		hd->heredoc_fds = ft_calloc(1, sizeof(int));
	else
		hd->heredoc_fds = ft_recalloc(hd->heredoc_fds,
				sizeof(int) * hd->num_heredocs,
				sizeof(int) * (hd->num_heredocs + 1));
	if (!hd->heredoc_fds)
	{
		free_array(&args);
		return (1);
	}
	return (0);
}

static int	save_heredoc_fds(t_shell *sh, char **args, int *expand, int i)
{
	sh->hd->heredoc_fds[sh->hd->num_heredocs]
		= here_doc_input(args[i + 1], sh, *expand);
	if (sh->hd->heredoc_fds[sh->hd->num_heredocs] == -1)
	{
		free_array(&args);
		return (1);
	}
	return (0);
}

static int	loop_args(t_shell *sh, char **args,
	char **args_with_quotes, int *expand)
{
	int	i;

	i = 0;
	while (args[i])
	{
		*expand = 0;
		if (ft_strncmp(args[i], "<<\0", 3) == 0 && args[i + 1])
		{
			if ((ft_strncmp(args[i + 1], args_with_quotes[i + 1],
						ft_strlen(args[i + 1])) == 0)
				|| args_with_quotes[i + 1][0] == '\"')
				*expand = 1;
			if (allocate_heredocs_fds(sh->hd, args)
				|| save_heredoc_fds(sh, args, expand, i))
			{
				free_array(&args_with_quotes);
				return (1);
			}
			sh->hd->num_heredocs++;
			i += 2;
		}
		else
			i++;
	}
	return (0);
}

int	handle_here_doc(t_shell *sh, char *input)
{
	int		i;
	char	**args;
	char	**args_with_quotes;
	int		expand_flag;

	i = 0;
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
	if (loop_args(sh, args, args_with_quotes, &expand_flag))
		return (1);
	free_array(&args);
	free_array(&args_with_quotes);
	return (0);
}
