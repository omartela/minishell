/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 14:10:51 by omartela          #+#    #+#             */
/*   Updated: 2024/09/05 11:51:21 by omartela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>

typedef struct s_shell
{
	char	**commands;
	char	**envp;
}	t_shell;

typedef struct s_cmd
{
	char	**args;
	char	**path;
	char	*infile;
	char	*outfile;
	int		append;
}	t_cmd;

int		init_cmd(t_cmd **cmd, const char *command, char **envp);
int		parse_redirections(t_cmd *cmd, char **args);
int		execute_pipes(t_shell *sh);
void	test_split(char *input); //only for test
void	print_command(t_cmd *cmd); //only for test
void	free_shell(t_shell *sh);
char	**ft_split_args(char const *s, char c);
void	free_array_back(char **array, size_t i);
void	free_array(char **array);
void	free_cmd(t_cmd *cmd);

#endif
