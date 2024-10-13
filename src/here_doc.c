/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 12:56:24 by irychkov          #+#    #+#             */
/*   Updated: 2024/10/13 20:31:36 by irychkov         ###   ########.fr       */
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

static int	here_doc_input(char *delimiter, t_shell *sh)
{
	int		pipe_fd[2];
	char	*temp;
	char	*line;

	(void) sh; //temp solution, we probably will free it
	if (pipe(pipe_fd) == -1)
	{
		error_sys("pipe failed\n"); // free
		return (-1);
	}
	while (1)
	{
		ft_putstr_fd("> ", 1);
		line = get_next_line(0);
		if (!line)
			break;
		temp = split_and_parse(line, sh);
		if (!temp)
		{
			error_sys("split_and_parse failed\n");
			return (-1);
		}
		free(line);
		line = temp;
		if (!is_continue(line, delimiter))
			break ;
		write(pipe_fd[1], line, ft_strlen(line));
		free(line);
	}
	close(pipe_fd[1]);
	return (pipe_fd[0]);
}

int	handle_here_doc(t_shell *sh, char *input)
{
	int		i;
	char	**args;

	i = 0;
	args = split_args_remove_quotes(input, ' ');
	if (!args)
		return (1);
	while (args[i])
	{
		if (ft_strncmp(args[i], "<<\0", 3) == 0 && args[i + 1])
		{
			if (!sh->hd->heredoc_fds)
				sh->hd->heredoc_fds = malloc(sizeof(int));
			else
				sh->hd->heredoc_fds = ft_realloc(sh->hd->heredoc_fds, sizeof(int) * sh->hd->num_heredocs, sizeof(int) * (sh->hd->num_heredocs + 1));
			sh->hd->heredoc_fds[sh->hd->num_heredocs] = here_doc_input(args[i + 1], sh);
			if (sh->hd->heredoc_fds[sh->hd->num_heredocs] == -1)
				return (1);
			sh->hd->num_heredocs++;
			i += 2;
		}
		else
			i++;
	}
	free_array(args);
	return (0);
}