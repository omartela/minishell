/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 15:44:35 by omartela          #+#    #+#             */
/*   Updated: 2024/10/23 14:42:06 by irychkov         ###   ########.fr       */
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
	int		heredoc_index;
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

typedef struct s_expand_state
{
	int		in_single_quotes;
	int		in_double_quotes;
	int		i;
	char	*result;
}	t_expand_state;

typedef struct s_check
{
	int		or;
	int		redirect;
	int		ampersand;
	int		pipe;
	int		text;
}	t_check;

typedef struct s_redirection
{
	int		i;
	int		j;
	int		is_exit;
	t_shell	*sh;
	t_cmd	*cmd;
	char	**clean_args;
}	t_redirection;

// main functions
void	initialize_shell(t_shell *sh, char ***envp);
int		add_prompt(t_shell *sh, char *input);
char	*trim_spaces(char *str);
void	process_quotes(char **s, int *in_quotes, char *quote_type);
char	*expand_input(char *str, t_shell *sh);
int		init_cmd(t_cmd **cmd, char *command, t_shell *sh);
void	init_num_cmds(t_shell *sh);
char	*add_spaces(char *s);
int		init_pipes(t_pipes *pipes, int num_cmds);
void	execute_pipes(t_shell *sh);
void	execute_command(t_shell *sh, t_cmd *cmd, char **envp);
void	check_permissions(t_shell *sh, t_cmd *cmd, int is_abs_relative);

// free functions
void	close_sh_hd_fds(t_shell *sh, t_cmd *cmd);
void	free_array_back(char **array, size_t i);
void	free_array(char ***array);
void	free_shell(t_shell *sh);
void	free_partial(t_shell *sh);
void	free_cmd(t_cmd **cmd);
void	free_pipes(t_shell *sh);
void	exit_and_free(t_shell *sh, t_cmd *cmd, int status);

// errors
void	show_error(char *name, char *msg);
void	show_syntax_error(char *element);
void	show_error_free_cmd_exit(int code, char *name, char *msg, t_cmd *cmd);
int		show_error_return(int code, char *name, char *msg);
void	error_sys(char *msg);
void	error_dup(t_shell *sh, t_cmd *cmd);
void	error_execve_and_permission(t_shell *sh, t_cmd *cmd, int is_abs_rel);

// test functions
void	test_split_args_leave_quotes(char *input, char c);
void	test_split_args_remove_quotes(char *input, char c);
void	print_command(t_cmd *cmd);
void	test_echo_command(char *argv[], t_shell *shell);

//split arguments
char	**split_args_remove_quotes(char *s, char c);
char	**split_args_leave_quotes(char *s, char c);
void	process_quotes(char **s, int *in_quotes, char *quote_type);
char	**split_args_helper(char *s, size_t i, char **result,
			t_split_opts *opts);

//check syntax
int		check_syntax(char *input);
int		is_redirection_operator(char c);
int		handle_or(t_check *check, size_t *i);
int		handle_pipe(t_check *check);
int		handle_and(t_check *check, size_t *i);
int		handle_ampersand(t_check *check);
void	handle_text(t_check *check, char *input, size_t *i);
int		handle_first_redirect(t_check *check, char *input, size_t *i);
int		handle_second_redirect(char *input, size_t i);

//heredoc
int		is_heredoc(char *input);
int		handle_here_doc(t_shell *sh, char *input);
int		here_doc_input(char *delimiter, t_shell *sh, int expand_flag);

//parse redirection
int		parse_redirections(t_shell *sh, t_cmd *cmd, int is_exit);
int		handle_input_redirection(t_redirection *data);
int		handle_output_redirection(t_redirection *data, int append_flag);
void	handle_heredoc_redirection(t_redirection *data);
int		cleanup_on_error_redir(t_redirection *data, int error_code);

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
int		set_table(char ***table, const char *variable, const char *value);
int		add_table(char ***table, const char *variable, const char *value);
int		remove_table(char ***table, const char *variable);
int		append_table(char ***table, const char *variable, const char *value);
int		set_variables(t_shell *shell, char *variable, char *value);
char	**sort_table(char **envp);

// environment_utilities.c
size_t	calculate_table_size(char ***table);
int		find_index(char ***table, const char *var, int *found, size_t *i);
int		update_t_var(char ***t, const char *var, const char *val, size_t *i);
int		append_table_value(char ***table, size_t index, const char *value);

// init_env.c
void	copy_env(char **envp, t_shell *shell);

// set_environment.c
int		set_variables(t_shell *shell, char *variable, char *value);
int		set_table(char ***table, const char *var, const char *value);

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
char	*expand(char **envp, char *variable);

// unset command
int		unset(t_shell *sh, char **args);

// signals.c
int		init_signal(t_shell *sh);
int		reset_signals(t_shell *sh);

// update-underscore-variable
int		update_underscore(t_shell *sh, t_cmd *cmd);

#endif
