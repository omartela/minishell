/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 19:29:43 by irychkov          #+#    #+#             */
/*   Updated: 2024/09/05 14:34:22 by irychkov         ###   ########.fr       */
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
		if ((ft_strncmp(args[i], "<", 2) == 0 || ft_strncmp(args[i], ">", 2) == 0
			|| ft_strncmp(args[i], ">>", 3) == 0) && args[i + 1])
		{
			i = i + 2;
			continue ;
		}
		i++;
		arg_count++;
	}
	return (arg_count);
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
		if (ft_strncmp(args[i], "<", 2) == 0 && args[i + 1])
		{
			cmd->infile = ft_strdup(args[i + 1]); //probably I already have and should free previous
			if (!cmd->infile)
			{
				perror("ft_strdup failed");
				free_array_back(clean_args, j);
				return (1);
			}
			i += 2;
			continue ;
		}
		if (ft_strncmp(args[i], ">", 2) == 0 && args[i + 1])
		{
			cmd->outfile = ft_strdup(args[i + 1]); //probably I already have and should free previous
			if (!cmd->outfile)
			{
				perror("ft_strdup failed");
				free_array_back(clean_args, j);
				return (1);
			}
			cmd->append = 0;
			i += 2;
			continue ;
		}
		if (ft_strncmp(args[i], ">>", 3) == 0 && args[i + 1])
		{
			cmd->outfile = ft_strdup(args[i + 1]); //probably I already have and should free previous
			if (!cmd->outfile)
			{
				perror("ft_strdup failed");
				free_array_back(clean_args, j);
				return (1);
			}
			cmd->append = 1;
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
