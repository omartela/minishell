/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utilities.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 18:02:33 by irychkov          #+#    #+#             */
/*   Updated: 2024/10/29 16:21:18 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_continue(char *line, char *delimiter)
{
	if (ft_strlen(delimiter) == 0 && line[0] == '\0')
	{
		free(line);
		return (0);
	}
	if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0
		&& line[ft_strlen(delimiter)] == '\0')
	{
		free(line);
		return (0);
	}
	return (1);
}

static int	close_fd_and_return(int fd0, int fd1, int error)
{
	close(fd0);
	close(fd1);
	return (error);
}

static int	add_promt_and_expand(t_shell *sh, char **line, int *fd,
	int expand_flag)
{
	char	*temp;

	if (add_prompt(sh, *line) || add_prompt(sh, "\n"))
	{
		free(*line);
		error_sys("add_prompt failed\n");
		return (close_fd_and_return(fd[0], fd[1], 1));
	}
	if (expand_flag)
	{
		temp = expand_input(*line, sh);
		free(*line);
		if (!temp)
		{
			error_sys("expand_input failed\n");
			return (close_fd_and_return(fd[0], fd[1], 1));
		}
		*line = temp;
	}
	return (0);
}

static int	setup_pipe_and_prompt(int *pipe_fd, t_shell *sh)
{
	if (pipe(pipe_fd) == -1)
		return (-1);
	if (add_prompt(sh, "\n"))
	{
		error_sys("add_prompt failed\n");
		return (close_fd_and_return(pipe_fd[0], pipe_fd[1], -1));
	}
	return (0);
}

int	here_doc_input(char *delimiter, t_shell *sh, int expand_flag)
{
	int		pipe_fd[2];
	char	*line;

	if (setup_pipe_and_prompt(pipe_fd, sh) == -1)
		return (-1);
	while (1)
	{
		/* line = readline("heredoc> "); */
		if (isatty(fileno(stdin)))
			line = readline("heredoc> ");
		else
		{
			char *input;
			input = get_next_line(fileno(stdin));
			line = ft_strtrim(input, "\n");
			free(input);
		}
		if (!line)
		{
			/* printf("warning: here-document delimited by \
end-of-file (wanted `%s')\n", delimiter); */
			break ;
		}
		if (add_promt_and_expand(sh, &line, pipe_fd, expand_flag))
			return (-1);
		if (!is_continue(line, delimiter))
			break ;
		write(pipe_fd[1], line, ft_strlen(line));
		write(pipe_fd[1], "\n", 1);
		free(line);
	}
	close(pipe_fd[1]);
	return (pipe_fd[0]);
}
