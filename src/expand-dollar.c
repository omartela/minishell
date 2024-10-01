/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand-dollar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 10:34:04 by omartela          #+#    #+#             */
/*   Updated: 2024/10/01 10:02:10 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int  is_check_dollar_sign(char *arg)
{
    if (ft_strchr(arg, '$'))
        return (1);
    else
        return (0);
}

static char    *expand(char **envp, char *variable)
{
    int     i;
    char    *value;

    i = 0;
    while (envp[i])
    {
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

void is_expandable(t_cmd *cmd)
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
}

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
            temp = ft_strjoin(result, str + ft_strlen(delimiter));
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

char    *split_and_parse(char *str, t_shell *sh)
{
    char    **table;
    int     i;
    char    *key;
    char    *insert;
    char    *newstr;
    char    *result;
    int    counter;

    i = 0;
    key = NULL;
    insert = NULL;
    newstr = NULL;
    result = NULL;
    table = ft_split(str, '$');
    if (!table)
        return (NULL);
    while (table[i])
    {
        counter = 0;
        if (table[i][0] == '?')
        {
            insert = get_exit_code(sh);
            if (!insert)
            {
                ft_putstr_fd("Parse dollar failed \n", 2);
                free_array(table);
                return (NULL);
            }
            newstr = insert_to_string(&table[i][0], "?", insert);
            if (!newstr)
            {
                free(insert);
                free_array(table);
                return (NULL);
            }
            free(insert);
            free(table[i]);
            table[i] = newstr;
        }
        else
        {
            while (ft_isalnum(table[i][counter]) || table[i][counter] == '_' || table[i][counter] == '{' || table[i][counter] == '}')
                ++counter;
            key = ft_substr(table[i], 0, counter);
            key = ft_strtrim(key, "{}");
            if (!key)
            {
                ft_putstr_fd("Parse dollar failed \n", 2);
                free_array(table);
                return (NULL);
            }
            insert = expand(sh->envp, key);
            if (!insert)
            {
                free(key);
                free_array(table);
                ft_putstr_fd("Parse dollar failed \n", 2);
                return (NULL);
            }
            newstr = insert_to_string(&table[i][0], key, insert);
            if (!newstr)
            {
                free(key);
                free(insert);
                free_array(table);
                return (NULL);
            }
            else
            {
                free(insert);
                free(key);
                free(table[i]);
                table[i] = newstr;
            }
        }
        ++i;
    }
    i = 1;
    if (table[0])
    {
        result = ft_strdup(table[0]);
        while (table[i])
        {
            result = ft_strjoin(result, table[i]);
            ++i;
        }
    }
    free_array(table);
    return (result);
}

int parse_dollar_sign(t_cmd *cmd, t_shell *sh)
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
}
