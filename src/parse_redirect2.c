/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirect2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 11:09:45 by irychkov          #+#    #+#             */
/*   Updated: 2024/10/23 12:09:53 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_input_redirection(t_redirection *data)
{
	int error_code;
	
	error_code = 0;
	if (data->cmd->fd_in != STDIN_FILENO)
	{
		close(data->cmd->fd_in);
		data->cmd->fd_in = STDIN_FILENO;
	}
	if (data->cmd->infile)
	{
		free(data->cmd->infile);
		data->cmd->infile = NULL;
	}
	error_code = open_fdin(data->cmd->args[data->i + 1], data->cmd);
	if (error_code)
	{
		free_array_back(data->clean_args, data->j);
		data->clean_args = NULL;
		if (data->is_exit)
			exit_and_free(data->sh, data->cmd, error_code);
		return (error_code);
	}
	data->cmd->infile = ft_strdup(data->cmd->args[data->i + 1]);
	if (!data->cmd->infile)
	{
		error_sys("ft_strdup failed\n");
		free_array_back(data->clean_args, data->j);
		data->clean_args = NULL;
		if (data->is_exit)
			exit_and_free(data->sh, data->cmd, 1);
		return (1);
	}
	data->i += 2;
	return (0);
}

int	handle_output_redirection(t_redirection *data, int append_flag)
{
	int error_code;
	
	error_code = 0;
	data->cmd->append = append_flag;
	if (data->cmd->outfile)
	{
		free(data->cmd->outfile);
		data->cmd->outfile = NULL;
		close(data->cmd->fd_out);
	}
	error_code = open_fdout(data->cmd->args[data->i + 1], data->cmd);
	if (error_code)
	{
		free_array_back(data->clean_args, data->j);
		data->clean_args = NULL;
		if (data->is_exit)
			exit_and_free(data->sh, data->cmd, error_code);
		else
			return (error_code);
	}
	data->cmd->outfile = ft_strdup(data->cmd->args[data->i + 1]);
	if (!data->cmd->outfile)
	{
		error_sys("ft_strdup failed\n");
		free_array_back(data->clean_args, data->j);
		data->clean_args = NULL;
		if (data->is_exit)
			exit_and_free(data->sh, data->cmd, 1);
		else
			return (1);
	}
	data->i += 2;
	return (0);
}

void	handle_heredoc_redirection(t_redirection *data)
{
	if (data->cmd->fd_in != STDIN_FILENO)
	{
		close(data->cmd->fd_in);
		data->cmd->fd_in = STDIN_FILENO;
	}
	data->cmd->here_doc += 1;
	if (data->cmd->infile)
	{
		free(data->cmd->infile);
		data->cmd->infile = NULL;
	}
	data->cmd->fd_in = data->cmd->fd_heredoc[data->cmd->here_doc - 1];
	data->i += 2;
}