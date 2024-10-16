/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 15:44:35 by omartela          #+#    #+#             */
/*   Updated: 2024/10/16 20:22:30 by irychkov         ###   ########.fr       */
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
# include <limits.h>
# include <signal.h>

typedef struct s_heredoc
{
	int		num_heredocs;
	int 	heredoc_index;
	int		*heredoc_fds;
}	t_heredoc;

typedef struct s_pipes
{
	int		**fd;
	pid_t	*pid;
}	t_pipes;

typedef struct s_shell
{
	int					exit_status;
	int					num_cmds;
	char				**commands;
	char				**envp;
	char				*promt;
	char				*homepath;
	char				**local_shellvars;
	struct s_pipes		*pipes;
	struct s_heredoc	*hd;
	struct sigaction	org_sig_quit;
	struct sigaction	org_sig_int;
}	t_shell;

typedef struct s_cmd
{
	char	**args_withquotes;
	char	**args;
	char	**path;
	char	*infile;
	char	*outfile;
	int		append;
	int		here_doc;
	int		*saved_std;
	int		*fd_heredoc;
	int		fd_in;
	int		fd_out;
	int		*expandable;
	int		is_continue;
}	t_cmd;

typedef struct s_split_opts
{
	int		keep_quotes;
	char	delimiter;
}	t_split_opts;

// main functions
int		add_prompt(t_shell *sh, char *input);
char	*trim_spaces(char *str);
int		check_syntax(char *input);
int		is_heredoc(char *input);
size_t	ft_strcounter(char *s, char c);
void	process_quotes(char **s, int *in_quotes, char *quote_type);
char 	*split_and_parse(char *str, t_shell *sh);
char	**split_args_remove_quotes(char *s, char c);
char	**split_args_leave_quotes(char *s, char c);
char	**split_args_general(char *s, char c, int keep_quotes);
int		init_cmd(t_cmd **cmd, char *command, t_shell *sh);
void	init_num_cmds(t_shell *sh);
char	*ft_add_spaces(char *s);
int		handle_here_doc(t_shell *sh, char *input);
int		parse_redirections(t_shell *sh, t_cmd *cmd, int is_exit);
int		init_pipes(t_pipes *pipes, int num_cmds);
void	execute_pipes(t_shell *sh);
void	execute_command(t_shell *sh, t_cmd *cmd, char **envp);

// free functions
void	free_array_back(char **array, size_t i);
void	free_array(char ***array);
void	free_shell(t_shell *sh);
void	free_partial(t_shell *sh);
void	free_cmd(t_cmd *cmd);
void	free_pipes(t_shell *sh);
void	exit_and_free(t_shell *sh, t_cmd *cmd, int status);

// errors
void	show_error(char *name, char *msg);
void	show_error_free_cmd_exit(int code, char *name, char *msg, t_cmd *cmd);
int		show_error_return(int code, char *name, char *msg);
void	error_sys(char *msg);

// test functions
void	test_split_args_leave_quotes(char *input, char c);
void	test_split_args_remove_quotes(char *input, char c);
void	print_command(t_cmd *cmd);
void	test_echo_command(char *argv[], t_shell *shell);

// echo_command
int		echo(char *argv[]);

// utilities.c
char	*expand_tilde(t_shell *sh);
int		is_builtin(t_cmd *cmd);
int		execute_builtin(t_shell *sh, t_cmd *cmd, int is_in_pipe);
char	*get_key(char *args);
int		is_check_key_equal(char *args, const char *variable);
char	*get_value(char *args);

// environment.c
void	copy_env(char **envp, t_shell *shell);
int		set_table(char ***table, const char *variable, const char *value);
int		add_table(char ***table, const char *variable, const char *value);
char	**sort_table(char **envp);
int		remove_table(char ***table, const char *variable);
int		append_table(char ***table, const char *variable, const char *value);
int		set_variables(t_shell *shell, char *variable, char *value);

//exit command
int		exit_shell(t_shell *sh, t_cmd *cmd);

//export command
int		export(t_shell *shell, char **arguments);

//cd command
int		cd(t_shell *sh, char **args);

//env	command
int		env(t_shell *shell, char **arguments);

// pwd command
int		pwd(void);

// expand-dollar.c
int		parse_dollar_sign(t_cmd	*cmd, t_shell *sh);
void	is_expandable(t_cmd *cmd);
char    *expand(char **envp, char *variable);

// unset command
int		unset(t_shell *sh, char **args);

// signals.c
int		init_signal(t_shell *sh);
int		reset_signals(t_shell *sh);
#endif
