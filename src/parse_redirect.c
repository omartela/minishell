/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 19:29:43 by irychkov          #+#    #+#             */
/*   Updated: 2024/09/05 22:28:59 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_new_args_len(char **args)
{
	int i;
	int	arg_count;

	i = 0;
	arg_count = 0;
	while (args[i])
	{
		if ((ft_strncmp(args[i], "<\0", 2) == 0 || ft_strncmp(args[i], ">\0", 2) == 0
			|| ft_strncmp(args[i], ">>\0", 3) == 0) && args[i + 1])
		{
			i = i + 2;
			continue ;
		}
		i++;
		arg_count++;
	}
	return (arg_count);
}

static void	open_fdin(char *infile, t_cmd *cmd)
{
	if (access(infile, F_OK) == -1)
	{
		perror("No such file or directory");
		exit(127);
	}
	if (access(infile, R_OK) == -1)
	{
		perror("Permission denied");
		exit(126);
	}
	cmd->fd_in = open(infile, O_RDONLY);
	if (cmd->fd_in == -1)
	{
		perror("open");
		exit(1);
	}
}

static void open_fdout(char *outfile, t_cmd *cmd)
{
	cmd->fd_out = open(outfile, O_DIRECTORY);
	if (cmd->fd_out != -1)
	{
		perror("Is a directory");
		exit(126);
	}
	if (access(outfile, F_OK) == 0 && access(outfile, W_OK) == -1)
	{
		perror("Permission denied");
		exit(1);
	}
	if (cmd->append)
	{
		cmd->fd_out = open(outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (cmd->fd_out == -1)
		{
			perror("open");
			exit(1);
		}
	}
	else
	{
		cmd->fd_out = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (cmd->fd_out == -1)
		{
			perror("open");
			exit(1);
		}
	}
}

int	parse_redirections(t_cmd *cmd, char **args)
{
	int		i;
	int		j;
	int		arg_count;
	char	**clean_args;

	i = 0;
	j = 0;
	arg_count = count_new_args_len(args);
	clean_args = malloc(sizeof(char *) * (arg_count + 1));
	if (!clean_args)
	{
		perror("malloc failed"); //free all
		return (1);
	}
	while (args[i])
	{
		if (ft_strncmp(args[i], "<\0", 2) == 0 && args[i + 1])
		{
			if (cmd->infile)
			{
				free(cmd->infile);
				close(cmd->fd_in);
			}
			open_fdin(args[i + 1], cmd);
			cmd->infile = ft_strdup(args[i + 1]);
			if (!cmd->infile)
			{
				perror("ft_strdup failed");
				free_array_back(clean_args, j);
				return (1);
			}
			i += 2;
			continue ;
		}
		if (ft_strncmp(args[i], ">\0", 2) == 0 && args[i + 1])
		{
			cmd->append = 0;
			if (cmd->outfile)
			{
				free(cmd->outfile);
				close(cmd->fd_out);
			}
			open_fdout(args[i + 1], cmd);
			cmd->outfile = ft_strdup(args[i + 1]);
			if (!cmd->outfile)
			{
				perror("ft_strdup failed");
				free_array_back(clean_args, j);
				return (1);
			}
			i += 2;
			continue ;
		}
		if (ft_strncmp(args[i], ">>\0", 3) == 0 && args[i + 1])
		{
			cmd->append = 1;
			if (cmd->outfile)
			{
				free(cmd->outfile);
				close(cmd->fd_out);
			}
			open_fdout(args[i + 1], cmd);
			cmd->outfile = ft_strdup(args[i + 1]);
			if (!cmd->outfile)
			{
				perror("ft_strdup failed");
				free_array_back(clean_args, j);
				return (1);
			}
			i += 2;
			continue ;
		}
		clean_args[j] = ft_strdup(args[i]); //protect
		if (!clean_args[j])
		{
			perror("ft_strdup failed");
			free_array_back(clean_args, j);
			return (1);
		}
		j++;
		i++;
	}
	clean_args[j] = NULL;
	free_array(cmd->args);
	cmd->args = clean_args;
	return (0);
}
