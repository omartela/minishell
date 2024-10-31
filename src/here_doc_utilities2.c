/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utilities2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 11:53:12 by irychkov          #+#    #+#             */
/*   Updated: 2024/10/31 13:01:04 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	if (sh->hd->heredoc_fds[sh->hd->num_heredocs] == -2)
	{
		free_array(&args);
		return (-1);
	}
	return (0);
}

static int	needs_expansion(char **args, char **args_with_quotes, int index)
{
	if ((ft_strncmp(args[index + 1], args_with_quotes[index + 1],
				ft_strlen(args[index + 1])) == 0)
		|| args_with_quotes[index + 1][0] == '\"')
		return (1);
	return (0);
}

int	loop_args(t_shell *sh, char **args, char **args_with_quotes, int *expand)
{
	int	i;
	int	catch_error;

	i = 0;
	catch_error = 0;
	while (args[i])
	{
		*expand = 0;
		if (ft_strncmp(args[i], "<<\0", 3) == 0 && args[i + 1])
		{
			*expand = needs_expansion(args, args_with_quotes, i);
			if (allocate_heredocs_fds(sh->hd, args))
			{
				free_array(&args_with_quotes);
				return (1);
			}
			catch_error = save_heredoc_fds(sh, args, expand, i);
			if (catch_error)
			{
				free_array(&args_with_quotes);
				return (catch_error);
			}
			sh->hd->num_heredocs++;
			i += 2;
		}
		else
			i++;
	}
	return (0);
}
