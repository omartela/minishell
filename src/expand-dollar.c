/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand-dollar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 10:34:04 by omartela          #+#    #+#             */
/*   Updated: 2024/10/20 16:31:05 by irychkov         ###   ########.fr       */
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

static char  *get_exit_code(t_shell *sh)
{
    char    *tempitoa;

    tempitoa = ft_itoa(sh->exit_status);
    if (!tempitoa)
    {
        ft_putstr_fd("Parse dollar failed \n", 2);
        return (NULL);
    }
    return (tempitoa);
}

char    *append_characters(char *result, char *str, int i)
{
    char    *temp;
    char    *temp_str;

    temp_str = ft_strndup(&str[i], 1);
    temp = ft_strjoin(result, temp_str);
    free(result);
    if (!temp)
        result = NULL;
    else
        result = temp;
    return (result);
}

char    *handle_dollarvarname(t_shell *sh, char *result, char *str, int *i)
{
    int     key_len;
    char    *key;
    char    *insert;
    char    *temp;

    *i += 1;
    key_len = 0;
    while (ft_isalnum(str[*i + key_len]) || str[*i + key_len] == '_') // Get the length of the variable name
        key_len++;

    key = ft_substr(str, *i, key_len); // Extract the variable name
    if (!key)
    {
        free(result);
        return (NULL);
    }
    insert = expand(sh->envp, key); // Expand the variable
    free(key);
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
    i += key_len; // Move the index past the variable name
    return (result);
}

char    *handle_exit_code(t_shell *sh, char *result)
{
    char    *insert;
    char    *temp;

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

char    *handle_tilde(t_shell *sh, char *result, char *str, int i)
{
    char    *temp;

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

char    *handle_tilde_middle(t_shell *sh, char *result)
{
    char    *temp;

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

char    *expand_input(char *str, t_shell *sh)
{
    int		in_single_quotes;
    int     in_double_quotes;
    char    *result;
    char    *insert;
    int     key_len;
    char    *key;
    int     i;
    char    *temp;

    i = 0;
	in_single_quotes = 0;
    in_double_quotes = 0;
    result = ft_strdup(""); // Start with an empty string to build the result dynamically
    if (!result)
        return (NULL);
    while (str[i])
    {
        handle_quotes(str[i], &in_single_quotes, &in_double_quotes);
        if (i == 0 && (ft_strncmp(str, "~\0", 2) == 0  || ft_strncmp(&str[i], "~/", 2) == 0) && !in_double_quotes && !in_single_quotes)
        {
            result = handle_tilde(sh, result, str, i);
            return (result);
        }
        if (str[i + 1] == '~') 
        {
            if ((ft_strncmp(&str[i], " ~ ", 3) == 0 || ft_strncmp(&str[i], " ~\0", 3) == 0 || ft_strncmp(&str[i], " ~/", 3) == 0) && !in_double_quotes && !in_single_quotes)
                {
                    result = handle_tilde_middle(sh, result);
                    if (!result)
                        return (NULL);
                    i += 2;
                    continue;
                }
        }
        if (str[i] == '$' && !in_single_quotes) // We found a '$' sign
        {
            if (str[i + 1] == '?') // Handle $? (exit code)
            {
                result = handle_exit_code(sh, result);
                if (!result)
                    return (NULL);
                i += 2; // Skip the $ and ?
            }
            else if (ft_isdigit(str[i + 1]) && !in_double_quotes)
            {
                handle_quotes(str[i + 1], &in_single_quotes, &in_double_quotes);
                i += 2;
            }
			else if ((str[i + 1] == '\'' || str[i + 1] == '\"') && !in_double_quotes)
				i += 1;
            else if (ft_isalpha(str[i + 1]) || str[i + 1] == '_') // Handle $VAR_NAME
            {
                i++;
                key_len = 0;
                while (ft_isalnum(str[i + key_len]) || str[i + key_len] == '_') // Get the length of the variable name
                    key_len++;

                key = ft_substr(str, i, key_len); // Extract the variable name
                if (!key)
                {
                    free(result);
                    return (NULL);
                }
                insert = expand(sh->envp, key); // Expand the variable
                free(key);
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
                i += key_len; // Move the index past the variable nam
            }
            else
            {
                // If we encounter just a single '$' with no valid variable, append it as is
                temp = ft_strjoin(result, "$");
                free(result);
                if (!temp)
                    return (NULL);
                result = temp;
                i++;
            }
        }
        else
        {
            result = append_characters(result, str, i);
            if (!result)
                result = NULL;
            i++;
        }
    }
    return (result);
}

