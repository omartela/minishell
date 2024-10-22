/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_recalloc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omartela <omartela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 10:08:21 by omartela          #+#    #+#             */
/*   Updated: 2024/10/21 10:08:25 by omartela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	*ft_recalloc(void *ptr, size_t old_size, size_t new_size)
{
	void	*new_ptr;
	size_t	copy_size;

	new_ptr = ft_calloc(1, new_size);
	if (!new_ptr)
	{
		free(ptr);
		return (NULL);
	}
	if (old_size < new_size)
		copy_size = old_size;
	else
		copy_size = new_size;
	if (ptr)
	{
		ft_memcpy(new_ptr, ptr, copy_size);
		free(ptr);
	}
	return (new_ptr);
}