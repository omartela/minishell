/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand-dollar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 10:34:04 by omartela          #+#    #+#             */
/*   Updated: 2024/10/24 11:51:08 by omartela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	*handle_dollarvar(t_shell *sh, char *result, char *str, int *i)
{
	int		key_len;
	char	*insert;
	char	*temp;

	*i += 1;
	key_len = var_name_len(str, i);
	insert = extract_key_and_expand(sh, str, i, key_len);
	if (!insert)
	{
		free(result);
		return (NULL);
	}
	temp = ft_strjoin(result, insert);
	free(insert);
	free(result);
	if (!temp)
		return (NULL);
	result = temp;
	*i += key_len;
	return (result);
}

static char	*handle_exit_code(t_shell *sh, char *result)
{
	char	*insert;
	char	*temp;

	insert = get_exit_code(sh);
	if (!insert)
	{
		free(result);
		return (NULL);
	}
	temp = ft_strjoin(result, insert);
	free(insert);
	free(result);
	result = temp;
	return (result);
}

static char	*only_dollar(t_expand_state *state)
{
	char	*temp;

	temp = ft_strjoin(state->result, "$");
	free(state->result);
	if (!temp)
		return (NULL);
	state->result = temp;
	state->i += 1;
	return (state->result);
}

static void	dollar_helper(t_shell *sh, t_expand_state *state, char *str)
{
	if (str[state->i + 1] == '?')
	{
		state->result = handle_exit_code(sh, state->result);
		state->i += 2;
	}
	else if (ft_isdigit(str[state->i + 1]) && !state->in_d_quotes)
	{
		handle_quotes(str[state->i + 1], &state->in_s_quotes, &state->in_d_quotes);
		state->i += 2;
	}
	else if ((str[state->i + 1] == '\'' || str[state->i + 1] == '\"') \
	&& !state->in_d_quotes)
		state->i += 1;
	else if (ft_isalpha(str[state->i + 1]) || str[state->i + 1] == '_')
		state->result = handle_dollarvar(sh, state->result, str, &state->i);
	else
		state->result = only_dollar(state);
}

char	*handle_dollar(t_shell *sh, t_expand_state *state, char *str)
{
	if (str[state->i] == '$' && !state->in_s_quotes)
		dollar_helper(sh, state, str);
	else
	{
		state->result = append_characters(state->result, str, state->i);
		state->i += 1;
	}
	return (state->result);
}
