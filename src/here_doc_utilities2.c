/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utilities2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 21:57:57 by irychkov          #+#    #+#             */
/*   Updated: 2024/10/30 22:10:31 by irychkov         ###   ########.fr       */
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

static int	save_heredoc_fds(t_shell *sh, t_heredoc_args *hd_args,
		int *expand, int i)
{
	sh->hd->heredoc_fds[sh->hd->num_heredocs]
		= here_doc_input(sh, hd_args, hd_args->args[i + 1], *expand);
	if (sh->hd->heredoc_fds[sh->hd->num_heredocs] == -1)
	{
		free_array(&hd_args->args);
		return (1);
	}
	if (sh->hd->heredoc_fds[sh->hd->num_heredocs] == -2)
	{
		free_array(&hd_args->args);
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

static int	process_heredoc(t_shell *sh, t_heredoc_args *hd_args,
		int *expand, int index)
{
	int	catch_error;

	*expand = needs_expansion(hd_args->args, hd_args->args_with_quotes, index);
	if (allocate_heredocs_fds(sh->hd, hd_args->args))
	{
		free_array(&hd_args->args_with_quotes);
		return 1;
	}
	catch_error = save_heredoc_fds(sh, hd_args, expand, index);
	if (catch_error != 0)
	{
		free_array(&hd_args->args_with_quotes);
		return (catch_error);
	}
	sh->hd->num_heredocs++;
	return (0);
}

int	loop_args(t_shell *sh, t_heredoc_args *hd_args, int *expand)
{
	int	i;
	int	catch_error;

	i = 0;
	catch_error = 0;
	while (hd_args->args[i])
	{
		*expand = 0;
		if (ft_strncmp(hd_args->args[i], "<<\0", 3) == 0 && hd_args->args[i + 1])
		{
			catch_error = process_heredoc(sh, hd_args, expand, i);
			if (catch_error != 0)
				return (catch_error);
			i += 2;
		}
		else
			i++;
	}
	return (0);
}
