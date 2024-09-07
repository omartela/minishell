/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 15:29:22 by irychkov          #+#    #+#             */
/*   Updated: 2024/09/08 00:41:22 by irychkov         ###   ########.fr       */
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
				perror("ft_split"); //free all
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

int	init_cmd(t_cmd **cmd, const char *command, char **envp)
{
	*cmd = malloc(sizeof(t_cmd));
	if (!*cmd)
	{
		perror("malloc"); //free all
		return (1);
	}
	(*cmd)->fd_in = STDIN_FILENO;
	(*cmd)->fd_out = STDOUT_FILENO;
	(*cmd)->infile = NULL;
	(*cmd)->outfile = NULL;
	(*cmd)->append = 0;
	(*cmd)->args = ft_split_args(command, ' ');
	if (!(*cmd)->args)
	{
		perror("ft_split_args"); //free all
		free(*cmd);
		return (1);
	}
	if (parse_redirections(*cmd, (*cmd)->args) == 1)
	{
		free_array((*cmd)->args);
		free(*cmd);
		return (1);
	}
	if (path_init(*cmd, envp) == 1)
	{
		free_array((*cmd)->args);
		free(*cmd);
		return (1);
	}
	print_command(*cmd); //only for test
	return (0);
}
