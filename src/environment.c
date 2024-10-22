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

int	set_variables(t_shell *shell, char *variable, char *value)
{
	int	success1;
	int	success2;

	success1 = set_table(&shell->envp, variable, value);
	success2 = set_table(&shell->local_shellvars, variable, value);
	if (success1 || success2)
		return (1);
	if (success2 == 0)
	{
		sort_table(shell->local_shellvars);
	}
	return (0);
}

static int	append_table_value(char ***table, size_t index, const char *value)
{
	char	*temp;

	temp = ft_strjoin((*table)[index], value);
	if (!temp)
		return (1);
	free((*table)[index]);
	(*table)[index] = temp;
	return (0);
}

static int	change_table_value(char ***table, size_t index, char *value)
{
	free((*table)[index]);
	(*table)[index] = value;
	return (0);
}

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

static size_t	calculate_table_size(char ***table)
{
	size_t	size;

	size = 0;
	while (((*table)[size]))
		++size;
	return (size);
}

void	alloc_tables(t_shell *sh, char ***c_envp, char ***l_shvars, size_t i)
{
	*c_envp = ft_calloc(i, sizeof(char *) + 1);
	*l_shvars = ft_calloc(i, sizeof(char *) + 1);
	if (!c_envp || !l_shvars)
	{
		if (c_envp)
			free(c_envp);
		if (l_shvars)
			free(l_shvars);
		sh->envp = NULL;
		sh->local_shellvars = NULL;
		error_sys("Copy environment failed..\n");
		exit(1);
	}
}

void	copy_env(char **envp, t_shell *shell)
{
	size_t	sarray;
	char	**copied_envp;
	char	**local_shellvars;
	size_t	i;

	sarray = 0;
	i = 0;
	// think what to do if envp is "empty" or does not exist
	sarray = calculate_table_size(&envp);
	alloc_tables(shell, &copied_envp, &local_shellvars, sarray);
	i = 0;
	while (i < sarray)
	{
		copied_envp[i] = ft_strdup(envp[i]);
		if (!copied_envp[i])
		{
			free_array_back(copied_envp, i);
			free_array_back(local_shellvars, i);
			error_sys("Copy environment failed..\n");
			exit(1);
		}
		local_shellvars[i] = ft_strdup(envp[i]);
		if (!local_shellvars[i])
		{
			free_array_back(local_shellvars, i);
			free_array_back(copied_envp, i + 1);
			error_sys("Copy environment failed..\n");
			exit(1);
		}
		++i;
	}
	copied_envp[i] = NULL;
	local_shellvars[i] = NULL;
	local_shellvars = sort_table(local_shellvars);
	shell->envp = copied_envp;
	shell->local_shellvars = local_shellvars;
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
	}
	(*table)[i + 1] = NULL;
	return (0);
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
	{
		if (add_table(table, variable, value))
			return (1);
		return (0);
	}
	return (1);
}

static int	update_t_var(char ***t, const char *var, const char *val, size_t *i)
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

int	set_table(char ***table, const char *var, const char *value)
{
	size_t	i;
	size_t	len;
	char	l_c;
	char	*t_var;

	i = 0;
	len = ft_strlen(var);
	while ((*table)[i])
	{
		t_var = (*table)[i];
		l_c = (*table)[i][len];
		if (ft_strncmp(t_var, var, len + 1) == 0 && !value)
			return (0);
		if (ft_strncmp(t_var, var, len) == 0 && (l_c == '=' || l_c == '\0'))
		{
			if (update_t_var(table, var, value, &i))
				return (1);
			return (0);
		}
		++i;
	}
	if (add_table(table, var, value))
		return (1);
	return (0);
}
