/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 16:25:07 by irychkov          #+#    #+#             */
/*   Updated: 2024/06/12 21:27:34 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static size_t	ft_newline_counter(char *stack)
{
	size_t	counter;
	size_t	i;

	i = 0;
	counter = 0;
	while (stack[i])
	{
		if (stack[i] == '\n')
			counter++;
		i++;
	}
	return (counter);
}

static char	*ft_remove_line(char *stack, char *line)
{
	char	*new_stack;
	size_t	line_len;
	size_t	stack_len;

	line_len = ft_strlen_buf(line);
	stack_len = ft_strlen_buf(stack);
	if (line_len >= stack_len)
		return (free_stuff(&stack));
	new_stack = ft_strdup_buf(stack + line_len);
	free_stuff(&stack);
	return (new_stack);
}

static char	*ft_update_stack(char	**stack)
{
	char	*line;

	if (!(*stack))
		return (NULL);
	line = ft_extract_line(*stack);
	if (line)
		*stack = ft_remove_line(*stack, line);
	if (!line)
		free_stuff(&*stack);
	return (line);
}

char	*get_next_line(int fd)
{
	char			buffer[BUFFER_SIZE];
	static t_gnl	fd_arr[MAX_FD];
	ssize_t			bytes_read;

	if (BUFFER_SIZE <= 0 || fd < 0)
		return (NULL);
	bytes_read = 1;
	while (bytes_read)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read <= 0)
		{
			if (bytes_read == -1)
				fd_arr[fd].stack = free_stuff(&fd_arr[fd].stack);
			if (fd_arr[fd].stack && *fd_arr[fd].stack)
				return (ft_update_stack(&fd_arr[fd].stack));
			return (free_stuff(&fd_arr[fd].stack));
		}
		fd_arr[fd].stack = ft_strlcat_buf(fd_arr[fd].stack, buffer, bytes_read);
		if (fd_arr[fd].stack && ft_newline_counter(fd_arr[fd].stack))
			return (ft_update_stack(&fd_arr[fd].stack));
	}
	return (free_stuff(&fd_arr[fd].stack));
}
