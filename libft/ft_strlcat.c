/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 23:05:30 by irychkov          #+#    #+#             */
/*   Updated: 2024/04/24 09:55:39 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	dst_len;
	size_t	size_remain;
	size_t	size_total;

	i = 0;
	dst_len = ft_strlen(dst);
	size_total = dst_len + ft_strlen(src);
	if (dstsize == 0 || dstsize <= dst_len)
		return (ft_strlen(src) + dstsize);
	else
		size_remain = dstsize - dst_len - 1;
	while (src[i] && size_remain > i)
	{
		dst[dst_len + i] = src[i];
		i++;
	}
	dst[dst_len + i] = '\0';
	return (size_total);
}
