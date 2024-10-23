/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 22:15:36 by irychkov          #+#    #+#             */
/*   Updated: 2024/10/23 22:35:49 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_cmd_saved_std(t_cmd *cmd)
{
	if (!cmd || !cmd->saved_std)
		return ;
	if (cmd->saved_std[0] != -1)
		close(cmd->saved_std[0]);
	if (cmd->saved_std[1] != -1)
		close(cmd->saved_std[1]);
	free(cmd->saved_std);
	cmd->saved_std = NULL;
}

static void	free_cmd_files(t_cmd *cmd)
{
	if (!cmd)
		return ;
	free(cmd->infile);
	cmd->infile = NULL;
	free(cmd->outfile);
	cmd->outfile = NULL;
}

static void	free_cmd_heredoc(t_cmd *cmd)
{
	if (!cmd->fd_heredoc)
		return ;
	free(cmd->fd_heredoc);
	cmd->fd_heredoc = NULL;
}

void	free_cmd(t_cmd **cmd)
{
	if (!cmd || !(*cmd))
		return ;
	if ((*cmd)->args_withquotes)
		free_array(&(*cmd)->args_withquotes);
	if ((*cmd)->args)
		free_array(&(*cmd)->args);
	if ((*cmd)->path)
		free_array(&(*cmd)->path);
	free_cmd_files(*cmd);
	free_cmd_saved_std(*cmd);
	free_cmd_heredoc(*cmd);
	free(*cmd);
	*cmd = NULL;
}
