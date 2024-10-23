/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_utilities.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 20:59:50 by omartela          #+#    #+#             */
/*   Updated: 2024/10/23 10:45:34 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

size_t	calculate_table_size(char ***table)
{
	size_t	size;

	size = 0;
	while (((*table)[size]))
		++size;
	return (size);
}

int	find_index(char ***table, const char *var, int *found, size_t *i)
{
	size_t	size;
	int		keyok;

	size = 0;
	while ((*table)[size])
	{
		keyok = is_check_key_equal((*table)[size], var);
		if (keyok == 1)
		{
			*i = size;
			*found = 1;
			return (0);
		}
		else if (keyok == -1)
		{
			return (-1);
		}
		++size;
	}
	return (1);
}

static int	change_table_value(char ***table, size_t index, char *value)
{
	free((*table)[index]);
	(*table)[index] = value;
	return (0);
}

int	update_t_var(char ***t, const char *var, const char *val, size_t *i)
{
	char	*temp;

	temp = ft_strjoin(var, "=");
	if (!temp)
		return (1);
	change_table_value(t, *i, temp);
	if (val)
	{
		if (append_table_value(t, *i, val))
			return (1);
		return (0);
	}
	return (0);
}

int	append_table_value(char ***table, size_t index, const char *value)
{
	char	*temp;

	temp = ft_strjoin((*table)[index], value);
	if (!temp)
		return (1);
	free((*table)[index]);
	(*table)[index] = temp;
	return (0);
}
