/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utilities.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 18:02:33 by irychkov          #+#    #+#             */
/*   Updated: 2024/11/01 15:34:24 by irychkov         ###   ########.fr       */
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

int	close_fd_and_return(int fd0, int fd1, int error)
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

int	read_hd_lines(int *pipe_fd, t_shell *sh, char *delim, int expand_flag)
{
	char	*line;

	while (1)
	{
		line = readline("heredoc> ");
		if (g_sig == SIGINT)
			return (close_fd_and_return(pipe_fd[0], pipe_fd[1], -2));
		if (!line)
		{
			printf("warning: here-document delimited by \
end-of-file (wanted `%s')\n", delim);
			break ;
		}
		if (add_promt_and_expand(sh, &line, pipe_fd, expand_flag))
			return (-1);
		if (!is_continue(line, delim))
			break ;
		write(pipe_fd[1], line, ft_strlen(line));
		write(pipe_fd[1], "\n", 1);
		free(line);
	}
	close(pipe_fd[1]);
	return (0);
}
