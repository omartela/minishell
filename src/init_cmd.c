/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 15:29:22 by irychkov          #+#    #+#             */
/*   Updated: 2024/10/23 14:43:14 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	path_init(t_cmd *cmd, char **envp)
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
				error_sys("ft_split failed\n");
				return (1);
			}
			return (0);
		}
		i++;
	}
	cmd->path = NULL;
	return (0);
}

static int	allocate_cmd_heredocs(t_cmd *cmd, int j)
{
	if (!cmd->fd_heredoc)
	{
		cmd->fd_heredoc = ft_calloc(1, sizeof(int));
		if (!cmd->fd_heredoc)
		{
			error_sys("malloc failed for fd_heredoc\n");
			return (1);
		}
	}
	else
	{
		cmd->fd_heredoc = ft_recalloc(cmd->fd_heredoc,
				sizeof(int) * j, sizeof(int) * (j + 1));
		if (!cmd->fd_heredoc)
		{
			error_sys("ft_recalloc failed for fd_heredoc\n");
			return (1);
		}
	}
	return (0);
}

int	init_heredocs(t_shell *sh, t_cmd *cmd)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (cmd->args[i])
	{
		if (ft_strncmp(cmd->args_withquotes[i], "<<\0", 3) == 0)
		{
			if (allocate_cmd_heredocs(cmd, j))
				return (1);
			cmd->fd_heredoc[j] = sh->hd->heredoc_fds[sh->hd->heredoc_index];
			sh->hd->heredoc_index++;
			i += 2;
			j++;
		}
		else
			i++;
	}
	return (0);
}

int	init_cmd_args_and_path(t_cmd **cmd, char *command, t_shell *sh)
{
	char	*temp;

	temp = add_spaces(command);
	if (!(temp))
	{
		error_sys("malloc failed for add_spaces\n");
		return (1);
	}
	(*cmd)->args = split_args_remove_quotes(temp, ' ');
	(*cmd)->args_withquotes = split_args_leave_quotes(temp, ' ');
	if (!(*cmd)->args_withquotes || !(*cmd)->args)
	{
		error_sys("ft_split_args failed\n");
		free(temp);
		return (1);
	}
	free(temp);
	if (init_heredocs(sh, *cmd) == 1 || (path_init(*cmd, sh->envp) == 1))
		return (1);
	return (0);
}

int	init_cmd(t_cmd **cmd, char *command, t_shell *sh)
{
	*cmd = ft_calloc(1, sizeof(t_cmd));
	if (!*cmd)
	{
		error_sys("malloc failed for cmd\n");
		return (1);
	}
	(*cmd)->is_continue = 1;
	(*cmd)->saved_std = ft_calloc(2, sizeof(int));
	if (!(*cmd)->saved_std)
	{
		error_sys("malloc failed for saved_std\n");
		free(*cmd);
		*cmd = NULL;
		return (1);
	}
	(*cmd)->saved_std[0] = -1;
	(*cmd)->saved_std[1] = -1;
	if (init_cmd_args_and_path(cmd, command, sh) == 1)
	{
		free_cmd(cmd);
		return (1);
	}
	return (0);
}
