/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 15:44:35 by omartela          #+#    #+#             */
/*   Updated: 2024/09/10 19:44:23 by irychkov         ###   ########.fr       */
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

typedef struct s_pipes
{
	int		**fd;
	pid_t	*pid;
}	t_pipes;

// main functions
char	**ft_split_args(char *s, char c);
int		init_cmd(t_cmd **cmd, char *command, char **envp);
int		syntax_check(char **commands);
void	init_num_cmds(t_shell *sh);
char	*ft_add_spaces(char *s);
int		init_pipes(t_pipes *pipes, int num_cmds);
int		execute_pipes(t_shell *sh);
void	parse_redirections(t_cmd *cmd, char **args);
void	execute_command(t_cmd *cmd, char **envp);

// free functions
void	free_array_back(char **array, size_t i);
void	free_array(char **array);
void	free_shell(t_shell *sh);
void	free_cmd(t_cmd *cmd);
void	free_pipes(t_pipes *pipes, int num_cmds);

// errors
void	show_error_free_cmd(int code, char *name, char *msg, t_cmd *cmd);
void	error_sys(char *msg);

// test functions
void	test_split(char *input);
void	print_command(t_cmd *cmd);
void	test_echo_command(char *argv[], t_shell *shell);

// echo_command
void	echo_command(char *argv[]);

// utilities.c
char	*expand_tilde(t_shell *sh);

#endif
