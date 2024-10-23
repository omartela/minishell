/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirect2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 11:09:45 by irychkov          #+#    #+#             */
/*   Updated: 2024/10/23 12:40:47 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	open_fdin(char *infile, t_cmd *cmd)
{
	if (access(infile, F_OK) == -1)
		return (show_error_return(1, infile, "No such file or directory"));
	if (access(infile, R_OK) == -1)
		return (show_error_return(126, infile, "Permission denied"));
	cmd->fd_in = open(infile, O_RDONLY);
	if (cmd->fd_in == -1)
		return (show_error_return(1, infile, "No such file or directory"));
	return (0);
}

int	open_fdout(char *outfile, t_cmd *cmd)
{
	cmd->fd_out = open(outfile, O_DIRECTORY);
	if (cmd->fd_out != -1)
	{
		close(cmd->fd_out);
		return (show_error_return(1, outfile, "Is a directory"));
	}
	if (access(outfile, F_OK) == 0 && access(outfile, W_OK) == -1)
		return (show_error_return(1, outfile, "Permission denied"));
	if (cmd->append)
	{
		cmd->fd_out = open(outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (cmd->fd_out == -1)
			return (show_error_return(1, outfile, "No such file or directory"));
	}
	else
	{
		cmd->fd_out = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (cmd->fd_out == -1)
			return (show_error_return(1, outfile, "No such file or directory"));
	}
	return (0);
}

int	handle_input_redirection(t_redirection *data)
{
	int	error_code;

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
	int	error_code;

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
