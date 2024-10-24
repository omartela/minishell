/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 15:44:35 by omartela          #+#    #+#             */
/*   Updated: 2024/10/24 17:16:06 by irychkov         ###   ########.fr       */
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
	int		in_s_quotes;
	int		in_d_quotes;
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
void	process_input(t_shell *sh, char *input);
void	init_num_cmds(t_shell *sh);
int		init_cmd(t_cmd **cmd, char *command, t_shell *sh);
int		init_pipes(t_pipes *pipes, int num_cmds);
void	execute_pipes(t_shell *sh);
void	exec_child(t_shell *sh, t_cmd *cmd, int i);
int		exec_in_parent(t_shell *sh, t_cmd *cmd);
void	execute_command(t_shell *sh, t_cmd *cmd, char **envp);
void	check_permissions(t_shell *sh, t_cmd *cmd, int is_abs_relative);
void	wait_for_children(t_pipes *pipes, t_shell *sh);

// free functions
void	close_sh_hd_fds(t_shell *sh, t_cmd *cmd);
void	free_array_back(char **array, size_t i);
void	free_array(char ***array);
void	free_commands_and_promt(t_shell *sh);
void	free_envp_and_shellvars(t_shell *sh);
void	free_heredoc_fds(t_shell *sh);
void	free_homepath(t_shell *sh);
void	free_pipes(t_shell *sh);
void	free_shell(t_shell *sh);
void	free_partial(t_shell *sh);
void	free_cmd(t_cmd **cmd);
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

//process input
int		add_prompt(t_shell *sh, char *input);
char	*trim_spaces(char *str);
char	*add_spaces(char *s);
int		is_open_quote(char *str);
void	process_quotes(char **s, int *in_quotes, char *quote_type);
char	*expand_input(char *str, t_shell *sh);
int		handle_heredoc_if_needed(t_shell *sh, char *input);
int		trim_and_check_syntax(t_shell *sh, char **input);
int		expand_and_add_spaces(t_shell *sh, char **input);
int		handle_continued_input(t_shell *sh, char **input, int len);
int		join_input_with_next(t_shell *sh, char **input, char *next_input);

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

// utilities.c
int		is_builtin(t_cmd *cmd);
char	*get_key(char *args);
int		is_check_key_equal(char *args, const char *variable);
char	*get_value(char *args);
int		is_only_numbers(char *str);

// execute-builtin-command-utilities.c
int		in_pipe(int (*b_in)(t_shell *, char **), t_shell *sh, t_cmd *cmd);
int		not_in_pipe(int (*b_in)(t_shell *, char **), t_shell *sh, t_cmd *cmd);

// execute-builtin-exit-cd-export-env.c
int		execute_exit_builtin(t_shell *sh, t_cmd *cmd, int is_in_pipe);

// execute-builtin-commands.c
int		execute_builtin(t_shell *sh, t_cmd *cmd, int is_in_pipe);
int		execute_export_builtin(t_shell *sh, t_cmd *cmd, int is_in_pipe);
int		execute_env_builtin(t_shell *sh, t_cmd *cmd, int is_in_pipe);
int		execute_cd_builtin(t_shell *sh, t_cmd *cmd, int is_in_pipe);

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

// echo_command
int		echo(char *argv[]);

//exit command
int		exit_shell(t_shell *sh, t_cmd *cmd);

//builtin-export.c
int		export(t_shell *shell, char **arguments);

//cd-command.c
int		cd(t_shell *sh, char **args);

//cd-command-utilities.c
int		set_currentpwd(t_shell *sh);
int		set_oldpwd(t_shell *sh, char *oldpwd);

//env	command
int		env(t_shell *shell, char **arguments);

// pwd command
int		pwd(void);

// expand-dollar.c
char	*handle_dollar(t_shell *sh, t_expand_state *state, char *str);

//expand tilde.c
int		is_only_tilde(char *str, t_expand_state *state);
int		is_tilde_middle(char *str, t_expand_state *state);
char	*handle_only_tilde(t_shell *sh, char *result, char *str, int i);
char	*handle_tilde_middle(t_shell *sh, char *result);

//expand_utilities.c
char	*extract_key_and_expand(t_shell *sh, char *str, int *i, int key_len);
int		var_name_len(char *str, int *i);
char	*append_characters(char *result, char *str, int i);
char	*get_exit_code(t_shell *sh);
char	*expand(char **envp, char *variable);

//expand.c
void	handle_quotes(char c, int *in_single_quote, int *in_double_quote);
char	*parse_and_expand(t_shell *sh, t_expand_state *state, char *str);
char	*expand_input(char *str, t_shell *sh);

//builtin-export-argument-validation
int		is_valid_export_argument(char *variable, char *value, char *equal);

//builtin-export-utilities.c
int		display_local_shellvars(t_shell *shell);
int		export_add_local(t_shell *sh, char *variable);
int		export_add_both(t_shell *sh, char *variable, char *value);
int		export_append_both(t_shell *sh, char *variable, char *value);

// unset command
int		unset(t_shell *sh, char **args);

// signals.c
int		init_signal(t_shell *sh);
int		reset_signals(t_shell *sh);
int		change_signal_handler(void);

// update-underscore-variable
int		update_underscore(t_shell *sh, t_cmd *cmd);

// update_shlvl
int		update_shlvl(t_shell *sh);

#endif
