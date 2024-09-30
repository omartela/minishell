/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand-dollar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omartela <omartela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 10:34:04 by omartela          #+#    #+#             */
/*   Updated: 2024/09/27 10:34:06 by omartela         ###   ########.fr       */
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

static char  *expand_exit_status(char *dollar, t_shell *sh)
{
    char    *temp;
    char    *tempitoa;

    tempitoa = ft_itoa(sh->exit_status);
    if (!tempitoa)
    {
        ft_putstr_fd("Parse dollar failed \n", 2);
        return (NULL);
    }
    temp = ft_strjoin(tempitoa, dollar + 1);
    if (!temp)
    {
        free(tempitoa);
        ft_putstr_fd("Parse dollar failed \n", 2);
        return (NULL);
    }
    free(tempitoa);
    return (temp);
}

void is_expandable(t_cmd *cmd)
{
    int i;
    int j;

    i = 0;
    j = 0;
    while (cmd->args[i])
    {
        if (is_check_dollar_sign(cmd->args[i]) && cmd->args[i][0] != '\'')
            cmd->expandable[i] = 1;
        else
            cmd->expandable[i] = 0;
        ++i;
    }
}

int parse_dollar_sign(t_cmd *cmd, t_shell *sh)
{
    int     i;
    char    *dollar;
    int     counter;
    char    *temp;

    i = 0;
    counter = 0;
    while (cmd->args[i])
    {
        if (cmd->expandable[i])
        {
            dollar = ft_strchr(cmd->args[i], '$');
            if (*(dollar + 1) == '?')
            {
                temp = expand_exit_status(dollar, sh);
                if (!temp)
                {
                    ft_putstr_fd("Parse dollar failed \n", 2);
                    return (1);
                }
                free(cmd->args[i]);
                cmd->args[i] = temp;
                return (1);
            }
            counter = 0;
            while (ft_isalnum(dollar[counter + 1]) || dollar[counter + 1] == '_')
                ++counter;
            temp = ft_substr(dollar, 1, counter);
            if (!temp)
            {
                ft_putstr_fd("Parse dollar failed \n", 2);
                return (1);
            }
            temp = expand(sh->envp, temp);
            if (!temp)
            {
                ft_putstr_fd("Parse dollar failed \n", 2);
                return (1);
            }
            if (ft_strncmp((dollar + 1), temp, counter) != 0)
            {
                free(cmd->args[i]);
                cmd->args[i] = temp;
            }
            else
                free(temp);
        }
        ++i;
    }
    return (0);
}
