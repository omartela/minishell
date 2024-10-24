/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 16:57:46 by omartela          #+#    #+#             */
/*   Updated: 2024/10/17 16:35:22 by omartela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	**sort_table(char **envp)
{
	char	*temp;
	int		i;

	i = 1;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], envp[i - 1], ft_strlen(envp[i])) < 0)
		{
			temp = envp[i - 1];
			envp[i - 1] = envp[i];
			envp[i] = temp;
			i = 0;
		}
		i++;
	}
	return (envp);
}

int	add_value_to_table(char ***table, size_t i, const char *value)
{
	if (append_table_value(table, i, "="))
	{
		(*table)[i + 1] = NULL;
		return (1);
	}
	if (append_table_value(table, i, value))
	{
		(*table)[i + 1] = NULL;
		return (1);
	}
	return (0);
}

int	add_table(char ***table, const char *variable, const char *value)
{
	size_t	i;
	char	**temp;

	i = 0;
	i = calculate_table_size(table);
	temp = ft_recalloc(*table, i * sizeof(char *), (i + 2) * sizeof(char *));
	if (!temp)
		return (1);
	*table = temp;
	(*table)[i] = ft_strdup(variable);
	if (!(*table)[i])
	{
		free((*table)[i + 1]);
		return (1);
	}
	if (value)
	{
		if (add_value_to_table(table, i, value))
			return (1);
	}
	(*table)[i + 1] = NULL;
	return (0);
}

int	remove_table(char ***table, const char *variable)
{
	size_t	index_to_remove;
	size_t	i;
	size_t	size;
	int		found;
	int		index_ok;

	i = 0;
	index_to_remove = 0;
	found = 0;
	index_ok = find_index(table, variable, &found, &index_to_remove);
	if (index_ok == -1)
		return (-1);
	size = calculate_table_size(table);
	if (found)
	{
		free((*table)[index_to_remove]);
		i = index_to_remove;
		while (i < size - 1)
		{
			(*table)[i] = (*table)[i + 1];
			++i;
		}
		(*table)[size - 1] = NULL;
	}
	return (0);
}

int	append_table(char ***table, const char *variable, const char *value)
{
	size_t	index_to_modify;
	int		found;
	int		index_ok;

	found = 0;
	index_ok = find_index(table, variable, &found, &index_to_modify);
	if (index_ok == -1)
		return (1);
	if (found)
	{
		if (!ft_strchr((*table)[index_to_modify], '='))
		{
			if (append_table_value(table, index_to_modify, "="))
				return (1);
		}
		if (append_table_value(table, index_to_modify, value))
			return (1);
		return (0);
	}
	else
		return (add_table(table, variable, value));
	return (1);
}
