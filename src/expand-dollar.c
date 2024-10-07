/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand-dollar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 10:34:04 by omartela          #+#    #+#             */
/*   Updated: 2024/10/07 17:16:17 by omartela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/* static int  is_check_dollar_sign(char *arg)
{
    if (ft_strchr(arg, '$'))
        return (1);
    else
        return (0);
} */

static char	*expand(char **envp, char *variable)
{
    int     i;
    char    *value;

    i = 0;
    while (envp[i])
    {
		if (ft_strncmp("BASHPID", variable, ft_strlen(variable) + 1) == 0)
		{
			value = ft_get_pid();
			if (!value)
				return (NULL);
			return (value);
		}
        if (is_check_key_equal(envp[i], variable))
        {
            value = get_value(envp[i]);
            if (!value)
                return (NULL);
            return (value);
        }
        ++i;
    }
    return (ft_strdup(""));
}

/* void is_expandable(t_cmd *cmd)
{
    int i;

    i = 0;
    while (cmd->args[i])
    {
        if (is_check_dollar_sign(cmd->args[i]) && cmd->args[i][0] != '\'')
            cmd->expandable[i] = 1;
        else
            cmd->expandable[i] = 0;
        ++i;
    }
} */

char	*ft_strndup(const char *s1, size_t n)
{
	size_t	i;
	char	*dst;

	i = 0;
	dst = (char *)malloc(sizeof(char) * (n + 1));
	if (dst == NULL)
		return (NULL);
	while (i < n)
	{
		dst[i] = s1[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}

char    *insert_to_string(char *str, char *delimiter, char *insert)
{
    size_t  i;
    char    *result;
    char    *temp;

    i = 0;
	result = NULL;
    while (str[i])
    {
        if (ft_strncmp(&str[i], delimiter, ft_strlen(delimiter)) == 0)
        {
            result = ft_strndup(str, i);
            if (!result)
                return (NULL);
            temp = ft_strjoin(result, insert);
            if (!temp)
            {
                free(result);
                return (NULL);
            }
            free(result);
            result = temp;
            temp = ft_strjoin(result, &str[i] + ft_strlen(delimiter));
            if (!temp)
            {
                free(result);
                return (NULL);
            }
            free(result);
            result = temp;
			return (result);
        }
        ++i;
    }
    return (NULL);
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

/* static int is_only_one_char(char *str, int c)
{
    while (*str)
    {
        if (*str != c)
            return (0);
        ++str;
    }
    return (1);
} */

/* static int is_2_dollar_signs(char *str)
{
	while (*str)
	{
		if (*str == '$' && *(str + 1) == '$')
			return (1);
		++str;
	}
	return (0);
} */

char *split_and_parse(char *str, t_shell *sh)
{
    int		in_single_quotes;
    char    *result;
    char    *insert;
    char    *key;
    int     i;
    int     key_len;
    char    *temp;

    i = 0;
	in_single_quotes = 0;
    result = ft_strdup(""); // Start with an empty string to build the result dynamically
    if (!result)
        return (NULL);

    while (str[i])
    {
        if (str[i] == '\'')
			in_single_quotes = !in_single_quotes;
        if (str[i] == '$' && !in_single_quotes) // We found a '$' sign
        {
            if (str[i + 1] == '$') // Handle `$$` for special case (like a PID)
            {
                // Replace $$ with a placeholder value, e.g., "89867" for now
                insert = ft_strdup(sh->pid); // This should eventually be replaced by get_pid() or similar
                if (!insert)
                    return (NULL);
                temp = ft_strjoin(result, insert);
                free(insert);
                free(result);
                result = temp;
                i += 2; // Skip the two $$ signs
            }
            else if (str[i + 1] == '?') // Handle $? (exit code)
            {
                insert = get_exit_code(sh);
                if (!insert)
                    return (NULL);
                temp = ft_strjoin(result, insert);
                free(insert);
                free(result);
                result = temp;
                i += 2; // Skip the $ and ?
            }
            else if (ft_isalpha(str[i + 1]) || str[i + 1] == '_') // Handle $VAR_NAME
            {
                i++;
                key_len = 0;
                while (ft_isalnum(str[i + key_len]) || str[i + key_len] == '_') // Get the length of the variable name
                    key_len++;

                key = ft_substr(str, i, key_len); // Extract the variable name
                if (!key)
                    return (NULL);

                insert = expand(sh->envp, key); // Expand the variable
                free(key);
                if (!insert)
                    return (NULL);

                temp = ft_strjoin(result, insert); // Append the expanded value to result
                free(insert);
                free(result);
                result = temp;
                i += key_len; // Move the index past the variable name
            }
            else
            {
                // If we encounter just a single '$' with no valid variable, append it as is
                temp = ft_strjoin(result, "$");
                free(result);
                result = temp;
                i++;
            }
        }
        else
        {
            // Append regular characters to the result
            char temp_str[2] = {str[i], '\0'}; // Create a string with one character
            temp = ft_strjoin(result, temp_str);
            free(result);
            result = temp;
            i++;
        }
    }
    return result;
}

/* int parse_dollar_sign(t_cmd *cmd, t_shell *sh)
{
    int     i;
    char    *result;

    i = 0;
    while (cmd->args[i])
    {
        if (cmd->expandable[i])
        {
            result = split_and_parse(cmd->args[i], sh);
            if (!result)
                return (1);
            free(cmd->args[i]);
            cmd->args[i] = result;
        }
        ++i;
    }
    return (0);
} */
