/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omartela <omartela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 16:57:46 by omartela          #+#    #+#             */
/*   Updated: 2024/09/10 20:48:18 by omartela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../include/minishell.h"

char	**sort_table(char **envp)
{
	char	*temp;
	int			i;

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
void	copy_env(char **envp, t_shell *shell)
{
	size_t	sarray;
	char	**copied_envp;
	char	**local_shellvars;
	size_t	i;

	sarray = 0;
	i = 0;
	while (envp[sarray])
		sarray++;
	copied_envp = ft_calloc(sarray, sizeof(char *) + 1);
	local_shellvars = ft_calloc(sarray, sizeof(char *) + 1);
	if (!copied_envp || !local_shellvars)
	{
		shell->envp = NULL;
		shell->local_shellvars = NULL;
		perror("Copy environment failed..");
		exit(1);
	}
	i = 0 ;
	while (i < sarray)
	{
		copied_envp[i] = ft_strdup(envp[i]);
		local_shellvars[i] = ft_strdup(envp[i]);
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
	size_t	sarr;
	char	*temp;
	char	**temp_table;

	sarr = 0;
	while ((*table)[sarr])
		sarr++;
	temp_table = ft_realloc(*table, sarr * sizeof(char *), (sarr + 2) * sizeof(char *));
	if (!temp_table)
		return (1);
	*table = temp_table;
	(*table)[sarr] = ft_strdup(variable);
	if (value)
	{
		temp = (*table)[sarr];
		(*table)[sarr] = ft_strjoin(temp, "=");
		if (!(*table)[sarr])
			return (1);
		//free(temp);
		temp = (*table)[sarr];
		(*table)[sarr] = ft_strjoin(temp, value);
		//free(temp);
		if (!((*table))[sarr])
			return (1);
	}
	(*table)[sarr + 1] = NULL;
	return (0);
}

int	remove_table(char ***table, const char *variable)
{
	size_t	size;
	size_t	index_to_remove;
	size_t	i;
	size_t	len;
	int		found;

	size = 0;
	i = 0;
	index_to_remove = 0;
	found = 0;
	len = ft_strlen(variable);
	while ((*table)[size])
	{
		if (is_check_key_equal((*table)[size], variable))
		{
			index_to_remove = size;
			found = 1;
		}
		++size;
	}
	if (found)
	{
		free((*table)[index_to_remove]);
		i = index_to_remove;
		while (i < size - 1)
		{
			(*table)[i] = (*table)[i + 1];
			++i;
		}
    	// Null-terminate the new table
    	(*table)[size - 1] = NULL;
	}
	return (0);
}

int	append_table(char ***table, const char *variable, const char *value)
{
	size_t	size;
	size_t	i;
	int		index_to_modify;
	size_t	len;
	int		found;
	char	*temp;

	i = 0;
	found = 0;
	size = 0;
	len = ft_strlen(variable);
	while ((*table)[size])
	{
		if (is_check_key_equal((*table)[size], variable))
		{
			index_to_modify = size;
			found = 1;
			break;
		}
		++size;
	}
	if (found)
	{
		temp = (*table)[index_to_modify];
		if (!(*table)[index_to_modify])
			return (0);
		(*table)[index_to_modify] = ft_strjoin((*table)[index_to_modify], value);
		free(temp);
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

int	set_table(char ***table, const char *variable, const char *value)
{
	size_t	i;
	size_t	len;
	char	*temp;

	i = 0;
	len = ft_strlen(variable);
	while ((*table)[i])
	{
		if (ft_strncmp((*table)[i], variable, len + 1) == 0 && !value)
			return (0);
		if (ft_strncmp((*table)[i], variable, len) == 0 && ((*table)[i][len] == '=' || (*table)[i][len] == '\0'))
		{
			temp = (*table)[i];
			(*table)[i] = ft_strjoin(variable, "=");
			if (!(*table)[i])
				return (1);
			free(temp);
			if (value)
			{
				temp = (*table)[i];
				(*table)[i] = ft_strjoin((*table)[i], value);
				if (!(*table)[i])
					return (1);
				free(temp);
				return (0);
			}
			return (0);
		}
		++i;
	}
	return (add_table(table, variable, value));
}
