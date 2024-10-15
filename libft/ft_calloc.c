/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 12:31:54 by irychkov          #+#    #+#             */
/*   Updated: 2024/04/29 17:37:47 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	void	*b;
	size_t	total;

	total = count * size;
	if (count != 0 && size != 0 && (size != (total / count)))
		return (NULL);
	b = (void *)malloc(total);
	if (b)
		ft_bzero(b, total);
	return (b);
}
