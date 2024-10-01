/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 15:29:22 by irychkov          #+#    #+#             */
/*   Updated: 2024/10/01 10:18:53 by irychkov         ###   ########.fr       */
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

static int count_arguments(t_cmd *cmd)
{
	int i;

	i = 0;
	while(cmd->args[i])
		++i;
	return (i);
}

int	init_heredocs(t_shell *sh, t_cmd *cmd)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (cmd->args[i])
	{
		if (ft_strncmp(cmd->args[i], "<<", 2) == 0)
		{
			if (!cmd->fd_heredoc)
			{
				cmd->fd_heredoc = malloc(sizeof(int));
				if (!cmd->fd_heredoc)
				{
					error_sys("malloc failed\n");
					return (1);
				}
			}
			else
			{
				cmd->fd_heredoc = ft_realloc(cmd->fd_heredoc, sizeof(int) * j, sizeof(int) * (j + 1));
				if (!cmd->fd_heredoc)
				{
					error_sys("ft_realloc failed\n");
					return (1);
				}
			}
			cmd->fd_heredoc[j] = sh->heredoc_fds[sh->heredoc_index];
			sh->heredoc_index++;
			/* printf("fd_heredoc[%d] = %d\n", j, cmd->fd_heredoc[j]); */
			i += 2;
			j++;
		}
		else
			i++;
	}
	return (0);
}


int	init_cmd(t_cmd **cmd, char *command, t_shell *sh)
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
	(*cmd)->here_doc = 0;
	(*cmd)->fd_heredoc = NULL;
	(*cmd)->args = NULL;
	(*cmd)->args = 0;
	temp = ft_add_spaces(command);
	(*cmd)->args = split_args_leave_quotes(temp, ' ');
	(*cmd)->expandable = malloc(sizeof(int) * count_arguments(*cmd));
	if (!(*cmd)->expandable)
		return (1);
	is_expandable(*cmd);
	free_array((*cmd)->args);
	if (!(temp))
	{
		error_sys("malloc failed\n"); //free all
		free_cmd(*cmd);
		return (1);
	}
	(*cmd)->args = split_args_remove_quotes(temp, ' ');
	/* free(temp); */
	if (!(*cmd)->args || parse_dollar_sign(*cmd, sh))
	{
		error_sys("ft_split_args failed\n"); //free all
		free_cmd(*cmd);
		return (1);
	}
	if (init_heredocs(sh, *cmd) == 1)
		return (1);
	if (path_init(*cmd, sh->envp) == 1)
	{
		free_array((*cmd)->args);
		free_cmd(*cmd);
		return (1);
	}
	return (0);
}
