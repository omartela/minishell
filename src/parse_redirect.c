/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 19:29:43 by irychkov          #+#    #+#             */
/*   Updated: 2024/10/10 11:31:18 by irychkov         ###   ########.fr       */
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

static int	open_fdin(char *infile, t_cmd *cmd, int is_exit)
{
	if (access(infile, F_OK) == -1)
	{
		if (is_exit)
			show_error_free_cmd_exit(1, infile, "No such file or directory", NULL); //free cmd?
		else
			return (show_error_return(1, infile, "No such file or directory"));
	}
	if (access(infile, R_OK) == -1)
	{
		if (is_exit)
			show_error_free_cmd_exit(126, infile, "Permission denied", NULL);
		else
			return (show_error_return(126, infile, "Permission denied"));
	}
	cmd->fd_in = open(infile, O_RDONLY);
	if (cmd->fd_in == -1)
	{
		if (is_exit)
			show_error_free_cmd_exit(1, infile, "No such file or directory", NULL); //free cmd?
		else
			return (show_error_return(1, infile, "No such file or directory"));
	}
	return (0);
}

static int	open_fdout(char *outfile, t_cmd *cmd, int is_exit)
{
	cmd->fd_out = open(outfile, O_DIRECTORY);
	if (cmd->fd_out != -1)
	{
		if (is_exit)
			show_error_free_cmd_exit(1, outfile, "Is a directory", NULL);
		else
			return (show_error_return(1, outfile, "Is a directory"));
	}
	if (access(outfile, F_OK) == 0 && access(outfile, W_OK) == -1)
	{
		if (is_exit)
			show_error_free_cmd_exit(1, outfile, "Permission denied", NULL);
		else
			return (show_error_return(1, outfile, "Permission denied"));
	}
	if (cmd->append)
	{
		cmd->fd_out = open(outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (cmd->fd_out == -1)
		{
			if (is_exit)
				show_error_free_cmd_exit(1, outfile, "No such file or directory", NULL);
			else
				return (show_error_return(1, outfile, "No such file or directory"));;
		}
	}
	else
	{
		cmd->fd_out = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (cmd->fd_out == -1)
		{
			if (is_exit)
				show_error_free_cmd_exit(1, outfile, "No such file or directory", NULL);
			else
				return (show_error_return(1, outfile, "No such file or directory"));;
		}
	}
	return (0);
}

int	parse_redirections(t_cmd *cmd, char **args, char **args_withquotes, int is_exit)
{
	int		i;
	int		j;
	int		arg_count;
	char	**clean_args;
	int		error_code;

	i = 0;
	j = 0;
	error_code = 0;
	arg_count = count_new_args_len(args_withquotes);
	clean_args = malloc(sizeof(char *) * (arg_count + 1));
	if (!clean_args)
	{
		error_sys("malloc failed\n"); //free all
		exit (1);
	}
	while (args[i])
	{
		if (ft_strncmp(args_withquotes[i], "<\0", 2) == 0 && args[i + 1])
		{
			if (cmd->fd_in != STDIN_FILENO)
			{
				close(cmd->fd_in);
				cmd->fd_in = STDIN_FILENO;
			}
			if (cmd->infile)
			{
				free(cmd->infile);
				cmd->infile = NULL;
			}
			error_code = open_fdin(args[i + 1], cmd, is_exit);
			if (error_code)
				return (error_code);
			cmd->infile = ft_strdup(args[i + 1]);
			if (!cmd->infile)
			{
				error_sys("ft_strdup failed\n");
				free_array_back(&clean_args, j);
				exit (1);
			}
			i += 2;
			continue ;
		}
		if (ft_strncmp(args_withquotes[i], ">\0", 2) == 0 && args[i + 1])
		{
			cmd->append = 0;
			if (cmd->outfile)
			{
				free(cmd->outfile);
				cmd->outfile = NULL;
				close(cmd->fd_out);
			}
			error_code = open_fdout(args[i + 1], cmd, is_exit);
			if (error_code)
				return (error_code);
			cmd->outfile = ft_strdup(args[i + 1]);
			if (!cmd->outfile)
			{
				error_sys("ft_strdup failed\n");
				free_array_back(&clean_args, j);
				exit (1);
			}
			i += 2;
			continue ;
		}
		if (ft_strncmp(args_withquotes[i], ">>\0", 3) == 0 && args[i + 1])
		{
			cmd->append = 1;
			if (cmd->outfile)
			{
				free(cmd->outfile);
				cmd->outfile = NULL;
				close(cmd->fd_out);
			}
			error_code = open_fdout(args[i + 1], cmd, is_exit);
			if (error_code)
				return (error_code);
			cmd->outfile = ft_strdup(args[i + 1]);
			if (!cmd->outfile)
			{
				error_sys("ft_strdup failed\n");
				free_array_back(&clean_args, j);
				exit (1);
			}
			i += 2;
			continue ;
		}
		if (ft_strncmp(args_withquotes[i], "<<\0", 3) == 0 && args[i + 1])
		{
			if (cmd->fd_in != STDIN_FILENO)
			{
				close(cmd->fd_in);
				cmd->fd_in = STDIN_FILENO;
			}
			cmd->here_doc += 1;
			if (cmd->infile)
			{
				free(cmd->infile);
				cmd->infile = NULL;
			}
			cmd->fd_in = cmd->fd_heredoc[cmd->here_doc - 1];
			i += 2;
			continue ;
		}
		clean_args[j] = ft_strdup(args[i]); //protect
		if (!clean_args[j])
		{
			error_sys("ft_strdup failed\n");
			free_array_back(&clean_args, j);
			exit (1);
		}
		j++;
		i++;
	}
	clean_args[j] = NULL;
	free_array(cmd->args);
	cmd->args = clean_args;
	return (error_code);
}
