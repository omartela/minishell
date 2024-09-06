/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 15:44:35 by omartela          #+#    #+#             */
/*   Updated: 2024/09/06 18:03:56 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include <unistd.h>

typedef struct s_shell
{
	int		num_cmds;
	char	**commands;
	char	**envp;
	char	*homepath;
}	t_shell;

typedef struct s_cmd
{
	char	**args;
	char	**path;
	char	*infile;
	char	*outfile;
	int		append;
	int		fd_in;
	int		fd_out;
}	t_cmd;

int		init_cmd(t_cmd **cmd, const char *command, char **envp);
void	init_num_cmds(t_shell *sh);
int		parse_redirections(t_cmd *cmd, char **args);
int		execute_pipes(t_shell *sh);
void	test_split(char *input); //only for test
void	print_command(t_cmd *cmd); //only for test
void	free_shell(t_shell *sh);
char	**ft_split_args(char const *s, char c);
void	free_array_back(char **array, size_t i);
void	free_array(char **array);
void	free_cmd(t_cmd *cmd);

// test_echo_command.c
void	test_echo_command(char *argv[], t_shell *shell);

// echo_command
void	echo_command(char *argv[]);

// utilities.c
char	*expand_tilde(t_shell *sh);

#endif
