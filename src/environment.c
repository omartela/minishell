/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omartela <omartela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 16:57:46 by omartela          #+#    #+#             */
/*   Updated: 2024/09/10 16:57:50 by omartela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../include/minishell.h"

void	copy_env(char **envp, t_shell *shell)
{
	size_t	sarray;
	char	**copied_envp;
	int		i;

	i = 0;
	while (envp[sarray])
		sarray++;
	copied_envp = malloc(sizeof(char *) * sarray);
	if (!copied_envp)
	{
		shell->envp = NULL;
		perror("Copy environment failed..");
		exit(1);
	}
	while (copied_envp[i])
		copied_envp[i] = ft_strdup(envp[i]);
	shell->envp = copied_envp;
}

static int	add_new(t_shell *shell, const char *variable, const char *value)
{
	size_t	sarr;
	size_t	i;
	char	*temp;
	char	**envp;

	i = 0;
	sarr = 0;
	envp = shell->envp;
	while (envp[i++])
		sarr++;
	envp = ft_realloc(envp, sarr * sizeof(char *), (sarr + 1) * sizeof(char *));
	if (!envp)
		return (1);
	if (value)
	{
		envp[sarr + 1] = ft_strjoin(variable, "=");
		temp = envp[sarr + 1];
		envp[sarr + 1] = ft_strjoin(temp, value);
		free(temp);
	}
	else
		envp[sarr + 1] = ft_strdup(variable);
	return (0);
}

int	set_env(t_shell *shell, const char *variable, const char *value)
{
	size_t	i;
	size_t	len;
	char	*temp;

	i = 0;
	while (shell->envp[i])
	{
		len = ft_strlen(variable);
		if (ft_strncmp(shell->envp[i], variable, len) == 0)
		{
			temp = shell->envp[i];
			shell->envp[i] = ft_strjoin(variable, "=");
			free(temp);
			temp = shell->envp[i];
			shell->envp[i] = ft_strjoin(shell->envp[i], value);
			free(temp);
			return (0);
		}
		++i;
	}
	return (add_new(shell, variable, value));
}
