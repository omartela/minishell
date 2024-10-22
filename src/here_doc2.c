/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 18:02:33 by irychkov          #+#    #+#             */
/*   Updated: 2024/10/22 18:49:40 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	close_fd_and_return(int fd0, int fd1, int error)
{
	close(fd0);
	close(fd1);
	return (error);
}

int	add_promt_and_expand(t_shell *sh, char **line, int fd0, int fd1)
{
	char	*temp;

	if (add_prompt(sh, *line))
	{
		free(*line);
		error_sys("add_prompt failed\n");
		return (close_fd_and_return(fd0, fd1, 1));
	}
	if (expand)
	{
		temp = expand_input(*line, sh);
		free(*line);
		if (!temp)
		{
			error_sys("expand_input failed\n");
			return (close_fd_and_return(fd0, fd1, 1));
		}
		*line = temp;
	}
	return (0);
}

int	here_doc_input(char *delimiter, t_shell *sh, int expand)
{
	/* int		flag; */
	int		pipe_fd[2];
	char	*temp;
	char	*line;

	if (pipe(pipe_fd) == -1)
		return (-1);
	if (add_prompt(sh, "\n"))
	{
		error_sys("add_prompt failed\n");
		return (close_fd_and_return(pipe_fd[0], pipe_fd[1], -1));
	}
	/* flag = 0; */
	while (1)
	{
		//ft_putstr_fd("> ", 1);
		line = get_next_line(0);
		if (!line)
			break ;
		if (add_promt_and_expand(sh, line, pipe_fd[0], pipe_fd[1]))
			return (-1);
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
