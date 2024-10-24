/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 12:56:24 by irychkov          #+#    #+#             */
/*   Updated: 2024/10/20 17:08:24 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_heredoc(char *input)
{
	int	i;
	int	in_single_quotes;
	int	in_double_quotes;

	i = 0;
	in_single_quotes = 0;
	in_double_quotes = 0;
	while (input[i])
	{
		if (input[i] == '\"' && !in_single_quotes)
			in_double_quotes = !in_double_quotes;
		if (input[i] == '\'' && !in_double_quotes)
			in_single_quotes = !in_single_quotes;
		if (input[i] == '<' && !in_double_quotes && !in_single_quotes)
		{
			if (input[i + 1] && input[i + 1 ]== '<')
				return (1);
		}
		i++;
	}
	return (0);
}

static int	is_continue(char *line, char *delimiter)
{
	if (ft_strlen(delimiter) == 0 && ft_strlen(line) == 1
		&& line[0] == '\n')
	{
		free(line);
		return (0);
	}
	if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0
		&& line[ft_strlen(delimiter)] == '\n')
	{
		free(line);
		return (0);
	}
	return (1);
}

static int	here_doc_input(char *delimiter, t_shell *sh, int expand)
{
	/* int		flag; */
	int		pipe_fd[2];
	char	*temp;
	char	*line;

	(void) sh; //temp solution, we probably will free it
	if (pipe(pipe_fd) == -1)
	{
		error_sys("pipe failed\n"); // free
		return (-1);
	}
	if (add_prompt(sh, "\n"))
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		error_sys("add_prompt failed\n");
		return (-1);
	}
	/* flag = 0; */
	while (1)
	{
		//ft_putstr_fd("> ", 1);
		line = get_next_line(0);
		if (!line)
			break;
		if (add_prompt(sh, line))
		{
			free(line);
			close(pipe_fd[0]);
			close(pipe_fd[1]);
			error_sys("add_prompt failed\n");
			return (-1);
		}
		if (expand)
		{
			temp = expand_input(line, sh);
			free(line);
			if (!temp)
			{
				close(pipe_fd[0]);
				close(pipe_fd[1]);
				error_sys("expand_input failed\n");
				return (-1);
			}
			line = temp;
		}
		if (!is_continue(line, delimiter))
			break ;
		write(pipe_fd[1], line, ft_strlen(line));
		free(line);
		/* flag = 1; */
	}
	/* if (!flag)
		printf("warning: here-document delimited by end-of-file (wanted `%s')\n", delimiter); */
	close(pipe_fd[1]);
	return (pipe_fd[0]);
}

int	handle_here_doc(t_shell *sh, char *input)
{
	int		i;
	char	**args;
	char	**args_with_quotes;
	int		expand;

	i = 0;
	expand = 0;
	args = split_args_remove_quotes(input, ' ');
	if (!args)
		return (1);
	args_with_quotes = split_args_leave_quotes(input, ' ');
	if (!args_with_quotes)
	{
		free(args);
		return (1);
	}
	while (args[i])
	{
		expand = 0;
		if (ft_strncmp(args[i], "<<\0", 3) == 0 && args[i + 1])
		{
			if ((ft_strncmp(args[i + 1], args_with_quotes[i + 1], ft_strlen(args[i + 1])) == 0) || args_with_quotes[i + 1][0] == '\"')
				expand = 1;
			if (!sh->hd->heredoc_fds)
				sh->hd->heredoc_fds = ft_calloc(1, sizeof(int));
			else
				sh->hd->heredoc_fds = ft_recalloc(sh->hd->heredoc_fds, sizeof(int) * sh->hd->num_heredocs, sizeof(int) * (sh->hd->num_heredocs + 1));
			if (!sh->hd->heredoc_fds)
			{
				free_array(&args);
				free_array(&args_with_quotes);
				return (1);
			}
			sh->hd->heredoc_fds[sh->hd->num_heredocs] = here_doc_input(args[i + 1], sh, expand);
			if (sh->hd->heredoc_fds[sh->hd->num_heredocs] == -1)
			{
				free_array(&args);
				free_array(&args_with_quotes);
				return (1);
			}
			sh->hd->num_heredocs++;
			i += 2;
		}
		else
			i++;
	}
	free_array(&args);
	free_array(&args_with_quotes);
	return (0);
}