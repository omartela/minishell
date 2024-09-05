/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd-command.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omartela <omartela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 16:06:06 by omartela          #+#    #+#             */
/*   Updated: 2024/09/05 10:28:42 by omartela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../include/minishell.h"

char *get_env_value(char **envs, char *key)
{
	size_t	i;
	size_t	len;

	i = 0;
	len = ft_strlen(key);
	while (envs[i])
	{
		if (ft_strncmp(envs[i], key, len))
	}
}

char *get_env_key(char **envs, char *value)
{
	size_t	i;
	size_t	len;

	i = 0;
	len = ft_strlen(value);
	while (envs[i])
	{

	}
}

int	cd_command(char *str, char **envs)
{
	if (str == NULL)
	{
		if (chdir(""))
			printf("Error when changing directory");
	}
	else
	{
		if (chdir(str))
			printf("Error when changing directory");
	}
}

int	main(int argc, char *argv[], char **envs)
{
	(void)argc;
	(void)argv;
	cd_command("../");
}
