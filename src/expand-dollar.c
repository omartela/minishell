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
    return (variable);
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

void    parse_dollar_sign(char ***args, t_shell *sh)
{
    int     i;
    char    *dollar;
    int     counter;
    char    *temp;

    i = 1;
    counter = 0;
    while ((*args)[i])
    {
        if (is_check_dollar_sign((*args)[i]))
        {
            dollar = ft_strchr((*args)[i], '$');
            if (*(dollar + 1) == '?')
            {
                temp = expand_exit_status(dollar, sh);
                if (!temp)
                {
                    ft_putstr_fd("Parse dollar failed \n", 2);
                    return ;
                }
                free((*args)[i]);
                (*args)[i] = temp;
                return ;
            }
            counter = 0;
            while (ft_isalnum(dollar[counter + 1]) || dollar[counter + 1] == '_')
                ++counter;
            temp = ft_substr(dollar, 1, counter);
            if (!temp)
            {
                ft_putstr_fd("Parse dollar failed \n", 2);
                return ;
            }
            temp = expand(sh->envp, temp);
            if (!temp)
            {
                ft_putstr_fd("Parse dollar failed \n", 2);
                return ;
            }
            if (temp != (*args)[i])
            {
                free((*args)[i]);
                (*args)[i] = temp;
            }
        }
        ++i;
    }
}
