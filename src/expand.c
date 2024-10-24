/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omartela <omartela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 12:04:16 by omartela          #+#    #+#             */
/*   Updated: 2024/10/24 12:29:52 by omartela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	handle_quotes(char c, int *in_single_quote, int *in_double_quote)
{
	if (c == '\'' && !(*in_double_quote))
		*in_single_quote = !(*in_single_quote);
	if (c == '\"' && !(*in_single_quote))
		*in_double_quote = !(*in_double_quote);
}

char	*parse_and_expand(t_shell *sh, t_expand_state *state, char *str)
{
	while (str[state->i])
	{
		handle_quotes(str[state->i], &state->in_s_quotes, &state->in_d_quotes);
		if (is_only_tilde(str, state))
		{
			state->result = handle_only_tilde(sh, state->result, str, state->i);
			return (state->result);
		}
		if (str[state->i + 1] == '~')
		{
			if (is_tilde_middle(str, state))
			{
				state->result = handle_tilde_middle(sh, state->result);
				if (!state->result)
					return (NULL);
				state->i += 2;
				continue ;
			}
		}
		state->result = handle_dollar(sh, state, str);
		if (!state->result)
			return (NULL);
	}
	return (state->result);
}

char	*expand_input(char *str, t_shell *sh)
{
	t_expand_state	state;

	ft_memset(&state, 0, sizeof(t_expand_state));
	state.result = ft_strdup("");
	if (!state.result)
		return (NULL);
	state.result = parse_and_expand(sh, &state, str);
	return (state.result);
}
