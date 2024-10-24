/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utilities.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omartela <omartela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 11:58:51 by omartela          #+#    #+#             */
/*   Updated: 2024/10/24 11:59:18 by omartela         ###   ########.fr       */
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

char	*get_exit_code(t_shell *sh)
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
	while (ft_isalnum(str[*i + key_len]) || str[*i + key_len] == '_')
		key_len++;
	return (key_len);
}

char	*extract_key_and_expand(t_shell *sh, char *str, int *i, int key_len)
{
	char	*key;
	char	*insert;

	key = ft_substr(str, *i, key_len);
	if (!key)
		return (NULL);
	insert = expand(sh->envp, key);
	free(key);
	return (insert);
}
