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

	sarr = 0;
	while ((*table)[sarr])
		sarr++;
	*table = ft_realloc(*table, sarr * sizeof(char *), (sarr + 1) * sizeof(char *));
	if (!(*table))
		return (1);
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

int	set_table(char ***table, const char *variable, const char *value)
{
	size_t	i;
	size_t	len;
	char	*temp;

	i = 0;

	while ((*table)[i])
	{
		len = ft_strlen(variable);
		if (ft_strncmp((*table)[i], variable, len) == 0)
		{
			temp = (*table)[i];
			(*table)[i] = ft_strjoin(variable, "=");
			if (!(*table)[i])
				return (1);
			free(temp);
			temp = (*table)[i];
			(*table)[i] = ft_strjoin((*table)[i], value);
			if (!(*table)[i])
				return (1);
			free(temp);
			return (0);
		}
		++i;
	}
	return (add_table(table, variable, value));
}