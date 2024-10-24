/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tilde.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omartela <omartela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 12:06:18 by omartela          #+#    #+#             */
/*   Updated: 2024/10/24 12:30:25 by omartela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_only_tilde(char *str, t_expand_state *state)
{
	if (state->i == 0 && (ft_strncmp(str, "~\0", 2) == 0 || ft_strncmp(&str[state->i], "~/", 2) == 0))
	{
		if (!state->in_double_quotes && !state->in_single_quotes)
			return (1);
	}
	return (0);
}

int	is_tilde_middle(char *str, t_expand_state *state)
{
	if ((ft_strncmp(&str[state->i], " ~ ", 3) == 0 || ft_strncmp(&str[state->i], " ~\0", 3) == 0 || ft_strncmp(&str[state->i], " ~/", 3) == 0))
	{
		if (!state->in_double_quotes && !state->in_single_quotes)
			return (1);
	}
	return (0);
}

char	*handle_only_tilde(t_shell *sh, char *result, char *str, int i)
{
	char	*temp;

	temp = NULL;
	if (*(sh->homepath) != '\0')
		temp = ft_strjoin(result, sh->homepath);
	else
		temp = ft_strjoin(result, "~");
	free(result);
	result = temp;
	if (ft_strncmp(&str[i], "~/", 2) == 0)
	{
		temp = ft_strjoin(result, &str[1]);
		free(result);
		result = temp;
	}
	return (result);
}

char	*handle_tilde_middle(t_shell *sh, char *result)
{
	char	*temp;

	temp = NULL;
	if (*(sh->homepath) != '\0')
	{
		temp = ft_strjoin(result, " ");
		free(result);
		result = temp;
		temp = ft_strjoin(result, sh->homepath);
	}
	else
		temp = ft_strjoin(result, " ~");
	free(result);
	result = temp;
	return (result);
}
