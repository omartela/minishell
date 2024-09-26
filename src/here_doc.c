/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 12:56:24 by irychkov          #+#    #+#             */
/*   Updated: 2024/09/26 17:42:51 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_heredoc(char *input)
{
	if (ft_strncmp(input, "<<", 2) == 0)
		return (1);
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
	char	*line;

	(void) sh; //temp solution, we probably will free it
	if (pipe(pipe_fd) == -1)
		error_sys("pipe failed\n"); // free
	while (1)
	{
		ft_putstr_fd("> ", 1);
		line = get_next_line(0);
		if (!is_continue(line, delimiter))
			break ;
		write(pipe_fd[1], line, ft_strlen(line));
		free(line);
	}
	close(pipe_fd[1]);
	return (pipe_fd[0]);
}

void	handle_here_doc(t_shell *sh, char *input)
{
	int		i;
	int 	j;
	char	**args;

	i = 0;
	j = 0;
	while (sh->heredoc_fd[j])
		j++;
	args = split_args_remove_quotes(input, " ");
	while (args[i])
	{
		if (ft_strncmp(args[i], "<<\0", 3) == 0 && args[i + 1])
		{
			sh->heredoc_fd = ft_realloc(sh->heredoc_fd, j, j + 1);
			j += 1;
			sh->heredoc_fd[j] = here_doc_input(args[i + 1], sh);
			i += 2;
			continue ;
		}
		i++;
	}
}