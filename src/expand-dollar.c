/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand-dollar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 10:34:04 by omartela          #+#    #+#             */
/*   Updated: 2024/10/24 14:21:27 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*expand(char **envp, char *variable)
{
	int		i;
	char	*value;
	int		keyok;

	i = 0;
	while (envp[i])
	{
		keyok = is_check_key_equal(envp[i], variable);
		if (keyok == 1)
		{
			value = get_value(envp[i]);
			if (!value)
				return (NULL);
			return (value);
		}
		else if (keyok == -1)
			return (NULL);
		++i;
	}
	return (ft_strdup(""));
}

static char	*get_exit_code(t_shell *sh)
{
	char	*tempitoa;

	tempitoa = ft_itoa(sh->exit_status);
	if (!tempitoa)
	{
		ft_putstr_fd("Parse dollar failed \n", 2);
		return (NULL);
	}
	return (tempitoa);
}

char	*append_characters(char *result, char *str, int i)
{
	char	*temp;
	char	*temp_str;

	temp_str = ft_strndup(&str[i], 1);
	if (!temp_str)
	{
		free(result);
		return (NULL);
	}
	temp = ft_strjoin(result, temp_str);
	free(result);
	free(temp_str);
	if (!temp)
		result = NULL;
	else
		result = temp;
	return (result);
}

int	var_name_len(char *str, int *i)
{
	int	key_len;

	key_len = 0;
	while (ft_isalnum(str[*i + key_len]) || str[*i + key_len] == '_') // Get the length of the variable name
		key_len++;
	return (key_len);
}

char	*extract_key_and_expand(t_shell *sh, char *str, int *i, int key_len)
{
	char	*key;
	char	*insert;

	key = ft_substr(str, *i, key_len); // Extract the variable name
	if (!key)
		return (NULL);
	insert = expand(sh->envp, key); // Expand the variable
	free(key);
	return (insert);
}

char	*handle_dollarvarname(t_shell *sh, char *result, char *str, int *i)
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
	temp = ft_strjoin(result, insert); // Append the expanded value to result
	free(insert);
	free(result);
	if (!temp)
		return (NULL);
	result = temp;
	*i += key_len; // Move the index past the variable name
	return (result);
}

char	*handle_exit_code(t_shell *sh, char *result)
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

void	handle_quotes(char c, int *in_single_quote, int *in_double_quote)
{
	if (c == '\'' && !(*in_double_quote))
		*in_single_quote = !(*in_single_quote);
	if (c == '\"' && !(*in_single_quote))
		*in_double_quote = !(*in_double_quote);
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

char	*only_dollar(t_expand_state *state)
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

char	*handle_dollar(t_shell *sh, t_expand_state *state, char *str)
{
	if (str[state->i] == '$' && !state->in_single_quotes) // We found a '$' sign
	{
		if (str[state->i + 1] == '?') // Handle $? (exit code)
		{
			state->result = handle_exit_code(sh, state->result);
			state->i += 2; // Skip the $ and ?
		}
		else if (ft_isdigit(str[state->i + 1]) && !state->in_double_quotes)
		{
			handle_quotes(str[state->i + 1], &state->in_single_quotes, &state->in_double_quotes);
			state->i += 2;
		}
		else if ((str[state->i + 1] == '\'' || str[state->i + 1] == '\"') && !state->in_double_quotes)
			state->i += 1;
		else if (ft_isalpha(str[state->i + 1]) || str[state->i + 1] == '_') // Handle $VAR_NAME
			state->result = handle_dollarvarname(sh, state->result, str, &state->i);
		else
			state->result = only_dollar(state);
	}
	else
	{
		state->result = append_characters(state->result, str, state->i);
		state->i += 1;
	}
	return (state->result);
}

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

char	*parse_and_expand(t_shell *sh, t_expand_state *state, char *str)
{
	while (str[state->i])
	{
		handle_quotes(str[state->i], &state->in_single_quotes, &state->in_double_quotes);
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
				continue;
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
	t_expand_state state;

	ft_memset(&state, 0, sizeof(t_expand_state));
	state.result = ft_strdup("");
	if (!state.result)
		return (NULL);
	state.result = parse_and_expand(sh, &state, str);
	return (state.result);
}
