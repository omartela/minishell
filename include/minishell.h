/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 15:44:35 by omartela          #+#    #+#             */
/*   Updated: 2024/09/16 17:23:02 by irychkov         ###   ########.fr       */
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
# include <linux/limits.h>

typedef struct s_shell
{
	int		exit_status;
	int		num_cmds;
	char	**commands;
	char	**envp;
	char	**local_shellvars;
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

typedef struct s_split_opts
{
	int		keep_quotes;
	char	delimiter;
}	t_split_opts;

// main functions
char	*trim_spaces(char *str);
int		check_syntax(char *input);
size_t	ft_strcounter(char *s, char c);
void	process_quotes(char **s, int *in_quotes, char *quote_type);
char	**split_args_remove_quotes(char *s, char c);
char	**split_args_leave_quotes(char *s, char c);
char	**split_args_general(char *s, char c, int keep_quotes);
int		init_cmd(t_cmd **cmd, char *command, char **envp);
void	init_num_cmds(t_shell *sh);
char	*ft_add_spaces(char *s);
void	parse_redirections(t_cmd *cmd, char **args);
int		init_pipes(t_pipes *pipes, int num_cmds);
int		execute_pipes(t_shell *sh);
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
void	test_split_args_leave_quotes(char *input, char c);
void	test_split_args_remove_quotes(char *input, char c);
void	print_command(t_cmd *cmd);
void	test_echo_command(char *argv[], t_shell *shell);

// echo_command
void	echo_command(char *argv[]);

// utilities.c
char	*expand_tilde(t_shell *sh);
void	execute_builtin_command(t_cmd *cmd, t_shell *shell);

// environment.c
void	copy_env(char **envp, t_shell *shell);
int		set_table(char ***table, const char *variable, const char *value);
int		add_table(char ***table, const char *variable, const char *value);
char	**sort_table(char **envp);

//export command
int		export(t_shell *shell, char **arguments);

//cd command
int	cd(t_shell *sh, char **args);

//env	command
int		env(t_shell *shell, char **arguments);

// pwd command
int    pwd(void);

#endif
