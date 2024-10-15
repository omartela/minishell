/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_pid.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omartela <omartela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 16:35:31 by omartela          #+#    #+#             */
/*   Updated: 2024/10/07 16:55:37 by omartela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../include/minishell.h"

char *ft_get_pid()
{
    int     fd;
    char    *pid;
    char    *line;

    pid = NULL;
    fd = 0;

    fd = open("/proc/self/stat", O_RDONLY);
    if (fd == -1)
    {
        return (NULL);
    }
    line = get_next_line(fd);
    if (!line)
        return (NULL);
    pid = ft_substr(line, 0, (ft_strchr(line, ' ') - line));
    if (!pid)
        return (NULL);
    return (pid);
}
