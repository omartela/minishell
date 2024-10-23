/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 19:29:43 by irychkov          #+#    #+#             */
/*   Updated: 2024/10/23 12:20:02 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_new_args_len(char **args)
{
	int	i;
	int	arg_count;

	i = 0;
	arg_count = 0;
	while (args[i])
	{
		if ((ft_strncmp(args[i], "<\0", 2) == 0
				|| ft_strncmp(args[i], ">\0", 2) == 0
				|| ft_strncmp(args[i], ">>\0", 3) == 0
				|| ft_strncmp(args[i], "<<\0", 3) == 0) && args[i + 1])
		{
			i = i + 2;
			continue ;
		}
		i++;
		arg_count++;
	}
	return (arg_count);
}

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

int init_redirection(t_redirection *data, t_shell *sh, t_cmd *cmd, int is_exit)
{
	int	arg_count;

	data->sh = sh;
	data->cmd = cmd;
	data->i = 0;
	data->j = 0;
	data->is_exit = is_exit;
	arg_count = count_new_args_len(cmd->args_withquotes);
	data->clean_args = ft_calloc(sizeof(char *), (arg_count + 1));
	if (!data->clean_args)
	{
		error_sys("malloc failed for clean_args\n");
		if (is_exit)
			exit_and_free(sh, cmd, 1);
		return (1);
	}
	return (0);
}

int	parse_redirections(t_shell *sh, t_cmd *cmd, int is_exit)
{
	int				error_code;
	t_redirection	data;

	error_code = 0;
	if (init_redirection(&data, sh, cmd, is_exit))
		return (1);
	while (data.cmd->args[data.i])
	{
		if (ft_strncmp(data.cmd->args_withquotes[data.i], "<\0", 2) == 0 && data.cmd->args[data.i + 1])
			error_code = handle_input_redirection(&data);
		else if (ft_strncmp(data.cmd->args_withquotes[data.i], ">\0", 2) == 0 && data.cmd->args[data.i + 1])
			error_code = handle_output_redirection(&data, 0);
		else if (ft_strncmp(data.cmd->args_withquotes[data.i], ">>\0", 3) == 0 && data.cmd->args[data.i + 1])
			error_code = handle_output_redirection(&data, 1);
		else if (ft_strncmp(data.cmd->args_withquotes[data.i], "<<\0", 3) == 0 && data.cmd->args[data.i + 1])
			handle_heredoc_redirection(&data);
		else
		{
			data.clean_args[data.j] = ft_strdup(data.cmd->args[data.i]);
			if (!data.clean_args[data.j])
			{
				error_sys("ft_strdup failed\n");
				free_array_back(data.clean_args, data.j);
				data.clean_args = NULL;
				if (data.is_exit)
					exit_and_free(data.sh, data.cmd, 1);
				else
					return (1);
			}
			data.j++;
			data.i++;
		}
		if (error_code)
			return (error_code);

	}
	data.clean_args[data.j] = NULL;
	free_array(&data.cmd->args);
	data.cmd->args = data.clean_args;
	close_sh_hd_fds(data.sh, data.cmd);
	return (error_code);
}
