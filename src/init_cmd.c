/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 15:29:22 by irychkov          #+#    #+#             */
/*   Updated: 2024/09/26 14:21:31 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	path_init(t_cmd *cmd, char **envp)//or getenv("PATH")
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			cmd->path = ft_split(envp[i] + 5, ':');
			if (!cmd->path)
			{
				error_sys("ft_split failed\n"); //free all
				return (1);
			}
			return (0);
		}
		i++;
	}
	cmd->path = NULL;
	return (0);
}

void	init_num_cmds(t_shell *sh)
{
	int	i;

	i = 0;
	while (sh->commands[i] != NULL)
		i++;
	sh->num_cmds = i;
}

int	init_cmd(t_cmd **cmd, char *command, char **envp)
{
	char	*temp;

	*cmd = malloc(sizeof(t_cmd));
	if (!*cmd)
	{
		error_sys("malloc failed\n"); //free all
		return (1);
	}
	(*cmd)->fd_in = STDIN_FILENO;
	(*cmd)->fd_out = STDOUT_FILENO;
	(*cmd)->infile = NULL;
	(*cmd)->outfile = NULL;
	(*cmd)->append = 0;
	(*cmd)->args = NULL;
	(*cmd)->limiter = NULL;
	temp = ft_add_spaces(command);
	if (!(temp))
	{
		error_sys("malloc failed\n"); //free all
		free_cmd(*cmd);
		return (1);
	}
	(*cmd)->args = split_args_remove_quotes(temp, ' ');
	if (!(*cmd)->args)
	{
		error_sys("ft_split_args failed\n"); //free all
		free_cmd(*cmd);
		return (1);
	}
	if (path_init(*cmd, envp) == 1)
	{
		free_array((*cmd)->args);
		free_cmd(*cmd);
		return (1);
	}
	return (0);
}
